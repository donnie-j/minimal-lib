#include <stdint.h>

typedef union {
    double value;
    struct {
        uint32_t lo;
        uint32_t hi;
    } uns;
} union64_t;

/*
 * Get two 32 bit ints from a double.
 */
#define UNPACK_DOUBLE(high,low,d) {\
        union64_t u = {0}; \
        u.value = d; \
        high = u.uns.hi; \
        low  = u.uns.lo; \
    }

/*
 * Set a double from two 32 bit ints.
 */
#define PACK_DOUBLE(d,high,low) { \
        union64_t u = {0}; \
        u.uns.hi = high; \
        u.uns.lo = low; \
        d = u.value; \
    }

typedef union {
    float value;
    uint32_t word;
} union32_t;

/*
 * Get two 32 bit ints from a double.
 */
#define UNPACK_FLOAT(w,f) {\
        union32_t u = {0}; \
        u.value = f; \
        w = u.word; \
    }

/*
 * Set a double from two 32 bit ints.
 */
#define PACK_FLOAT(f,w) { \
        union32_t u = {0}; \
        u.word = w; \
        f = u.value; \
    }

static const double one = 1.0, Zero[] = {0.0, -0.0,};

double fmod(double x, double y)
{
    int32_t n=0, hx=0, hy=0, hz=0, ix=0, iy=0, sx=0, i=0;

    if (sizeof(float) == sizeof(double)) {
        /*
         * Double is 32-bit.
         */
        UNPACK_FLOAT(hx,x);
        UNPACK_FLOAT(hy,y);
        sx = hx & 0x80000000;                           /* sign of x */
        hx ^= sx;                                       /* |x| */
        hy &= 0x7fffffff;                               /* |y| */

        /* purge off exception values */
        if (hy == 0 || hx >= 0x7f800000 ||              /* y=0, or x not finite */
            hy > 0x7f800000)                            /* or y is NaN */
            return (x*y) / (x*y);
        if (hx < hy)
            return x;                                   /* |x| < |y| return x */
        if (hx == hy)
            return Zero[(uint32_t)sx >> 31];            /* |x| = |y| return x*0*/

        /* determine ix = ilogb(x) */
        if (hx < 0x00800000) {                          /* subnormal x */
            for (ix= -126, i=hx<<8; i>0; i<<=1)
                ix -= 1;
        } else
            ix = (hx >> 23) - 127;

        /* determine iy = ilogb(y) */
        if (hy < 0x00800000) {                          /* subnormal y */
            for (iy= -126, i=hy<<8; i>=0; i<<=1)
                iy -= 1;
        } else iy = (hy >> 23) - 127;

        /* set up {hx,lx}, {hy,ly} and align y to x */
        if (ix >= -126)
            hx = 0x00800000 | (0x007fffff & hx);
        else {                                          /* subnormal x, shift x to normal */
            n = -126 - ix;
            hx = hx << n;
        }
        if (iy >= -126)
            hy = 0x00800000 | (0x007fffff & hy);
        else {                                          /* subnormal y, shift y to normal */
            n = -126 - iy;
            hy = hy << n;
        }

        /* fix point fmod */
        n = ix - iy;
        while (n--) {
            hz = hx - hy;
            if (hz < 0) {
                hx = hx + hx;
            } else {
                if (hz == 0)                            /* return sign(x)*0 */
                    return Zero[(uint32_t)sx >> 31];
                hx = hz + hz;
            }
        }
        hz = hx - hy;
        if (hz >= 0) {
            hx = hz;
        }

        /* convert back to floating value and restore the sign */
        if (hx == 0)                                    /* return sign(x)*0 */
            return Zero[(uint32_t)sx >> 31];
        while (hx < 0x00800000) {                       /* normalize x */
            hx = hx + hx;
            iy -= 1;
        }
        if (iy >= -126) {                               /* normalize output */
            hx = (hx - 0x00800000) | ((iy + 127) << 23);
            PACK_FLOAT(x, hx | sx);
        } else {                                        /* subnormal output */
            n = -126 - iy;
            hx >>= n;
            PACK_FLOAT(x, hx | sx);
            x *= one;                                   /* create necessary signal */
        }
    } else {
        /*
         * Double is 64-bit.
         */
        uint32_t lx=0, ly=0, lz=0;

        UNPACK_DOUBLE(hx, lx, x);
        UNPACK_DOUBLE(hy, ly, y);
        sx = hx & 0x80000000;                           /* sign of x */
        hx ^= sx;                                       /* |x| */
        hy &= 0x7fffffff;                               /* |y| */

        /* purge off exception values */
        if ((hy | ly) == 0 || hx >= 0x7ff00000 ||       /* y=0,or x not finite */
            (hy | ((ly | -ly) >> 31)) > 0x7ff00000)     /* or y is NaN */
            return (x*y) / (x*y);
        if (hx <= hy) {
            if (hx < hy || lx < ly)
                return x;                               /* |x| < |y| return x */
            if (lx == ly)
                return Zero[(uint32_t)sx >> 31];        /* |x| = |y| return x*0 */
        }

        /* determine ix = ilogb(x) */
        if (hx < 0x00100000) {                          /* subnormal x */
            if (hx == 0) {
                for (ix = -1043, i=lx; i>0; i<<=1)
                    ix -= 1;
            } else {
                for (ix = -1022, i=hx<<11; i>0; i<<=1)
                    ix -= 1;
            }
        } else
            ix = (hx >> 20) - 1023;

        /* determine iy = ilogb(y) */
        if (hy < 0x00100000) {                          /* subnormal y */
            if(hy==0) {
                for (iy = -1043, i=ly; i>0; i<<=1)
                    iy -= 1;
            } else {
                for (iy = -1022,i=(hy<<11); i>0; i<<=1)
                    iy -= 1;
            }
        } else
            iy = (hy >> 20) - 1023;

        /* set up {hx,lx}, {hy,ly} and align y to x */
        if (ix >= -1022)
            hx = 0x00100000 | (0x000fffff & hx);
        else {                                          /* subnormal x, shift x to normal */
            n = -1022 - ix;
            if (n <= 31) {
                hx = (hx << n) | (lx >> (32 - n));
                lx <<= n;
            } else {
                hx = lx << (n - 32);
                lx = 0;
            }
        }
        if (iy >= -1022)
            hy = 0x00100000 | (0x000fffff & hy);
        else {                                          /* subnormal y, shift y to normal */
            n = -1022 - iy;
            if (n <= 31) {
                hy = (hy << n) | (ly >> (32 - n));
                ly <<= n;
            } else {
                hy = ly << (n - 32);
                ly = 0;
            }
        }

        /* fix point fmod */
        n = ix - iy;
        while (n--) {
            hz = hx - hy;
            lz = lx - ly;
            if (lx < ly)
                hz -= 1;

            if (hz < 0) {
                hx = hx + hx + (lx >> 31);
                lx = lx + lx;
            } else {
                if ((hz | lz) == 0)                     /* return sign(x)*0 */
                    return Zero[(uint32_t)sx >> 31];
                hx = hz + hz + (lz >> 31);
                lx = lz + lz;
            }
        }
        hz = hx - hy;
        lz = lx - ly;
        if (lx < ly)
            hz -= 1;
        if (hz >= 0) {
            hx = hz;
            lx = lz;
        }

        /* convert back to floating value and restore the sign */
        if ((hx | lx) == 0)                             /* return sign(x)*0 */
            return Zero[(uint32_t)sx >> 31];
        while (hx < 0x00100000) {                       /* normalize x */
            hx = hx + hx + (lx >> 31);
            lx = lx + lx;
            iy -= 1;
        }
        if (iy >= -1022) {                              /* normalize output */
            hx = (hx - 0x00100000) | ((iy + 1023) << 20);
            PACK_DOUBLE(x, hx | sx, lx);
        } else {                                        /* subnormal output */
            n = -1022 - iy;
            if (n <= 20) {
                lx = (lx >> n) | ((uint32_t)hx << (32 - n));
                hx >>= n;
            } else if (n <= 31) {
                lx = (hx << (32 - n)) | (lx >> n);
                hx = sx;
            } else {
                lx = hx >> (n - 32);
                hx = sx;
            }
            PACK_DOUBLE(x, hx | sx, lx);
            x *= one;                                   /* create necessary signal */
        }
    }
    return x;                                           /* exact output */
}
