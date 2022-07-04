#include <stddef.h>

#ifdef debug

#define ASSERT(p)
#else
#define ASSERT(p)

#endif	/* debug */

/*
 * C storage allocator
 *
 * Uses circular first-fit strategy.  Works with a noncontiguous, but
 * monotonically linked, arena.  Each block is preceded by a ptr to the
 * pointer of the next following block.  Blocks are exact number of words
 * long aligned to the data type requirements of ALIGN.
 *
 * Bit 0 (LSB) of pointers is used to indicate whether the block associated
 * with the pointer is in use.  A 1 indicates a busy block and a 0 a free
 * block (obviously pointers can't point at odd addresses).  Gaps in arena
 * are merely noted as busy blocks.  The last block of the arena (pointed
 * to by alloct) is empty and has a pointer to first.  Idle blocks are
 * coalesced during space search
 *
 * Different implementations may need to redefine ALIGN, NALIGN, BLOCK,
 * BUSY, INT where INT is integer type to which a pointer can be cast.
 */
#define	INT		int
#define	ALIGN		int
#define	NALIGN		1
#define	WORD		sizeof(union store)
#define	BLOCK		1024	/* a multiple of WORD */

#define	BUSY		1

#define	testbusy(p)	((INT)(p)&BUSY)
#define	setbusy(p)	(union store *)((INT)(p)|BUSY)
#define	clearbusy(p)	(union store *)((INT)(p)&~BUSY)

union store {
	union store	*ptr;
	ALIGN		dummy[NALIGN];
	int		calloc;	/* calloc clears an array of integers */
};

static union store	allocs[2];	/* initial arena */
static union store	*allocp;	/* search ptr */
static union store	*alloct;	/* arena top */
static union store	*allocx;	/* for benefit of realloc */

extern void * sbrk(int n);

void *
malloc(nbytes)
	size_t nbytes;
{
	register union store *p, *q;
	register int nw;
	static int temp;	/* coroutines assume no auto */

	if (nbytes == 0)
		return(NULL);
	if (allocs[0].ptr == 0) {	/* first time */
		allocs[0].ptr = setbusy(&allocs[1]);
		allocs[1].ptr = setbusy(&allocs[0]);
		alloct = &allocs[1];
		allocp = &allocs[0];
	}
	nw = (nbytes+WORD+WORD-1)/WORD;
	ASSERT(allocp >= allocs && allocp <= alloct);
	ASSERT(allock());
	for (p = allocp; ; ) {
		for (temp = 0; ; ) {
			if (!testbusy(p->ptr)) {
				while(!testbusy((q = p->ptr)->ptr)) {
					ASSERT(q > p && q < alloct);
					p->ptr = q->ptr;
				}
				if (q >= p+nw && p+nw >= p)
					goto found;
			}
			q = p;
			p = clearbusy(p->ptr);
			if (p > q)
				ASSERT(p <= alloct);
			else if (q != alloct || p != allocs) {
				ASSERT(q == alloct && p == allocs);
				return(NULL);
			} else if (++temp > 1)
				break;
		}
		q = (union store *)sbrk(0);
		/*
		 * Line up on page boundry so we can get the last drip at
		 * the end ...
		 */
		temp = ((((unsigned)q + WORD*nw + BLOCK-1)/BLOCK)*BLOCK
			- (unsigned)q) / WORD;
		if (q+temp < q)
			return(NULL);
		q = (union store *)sbrk(temp*WORD);
		if ((INT)q == -1)
			return(NULL);
		ASSERT(q > alloct);
		alloct->ptr = q;
		if (q != alloct+1)
			alloct->ptr = setbusy(alloct->ptr);
		alloct = q->ptr = q+temp-1;
		alloct->ptr = setbusy(allocs);
	}
found:
	allocp = p + nw;
	ASSERT(allocp <= alloct);
	if (q > allocp) {
		allocx = allocp->ptr;
		allocp->ptr = p->ptr;
	}
	p->ptr = setbusy(allocp);
	return((char *)(p+1));
}

/*
 * Freeing strategy tuned for LIFO allocation.
 */
void free(ap)
	register void *ap;
{
	register union store *p = (union store *)ap;

	if (p == NULL)
	        return;
	ASSERT(p > clearbusy(allocs[1].ptr) && p <= alloct);
	ASSERT(allock());
	allocp = --p;
	ASSERT(testbusy(p->ptr));
	p->ptr = clearbusy(p->ptr);
	ASSERT(p->ptr > allocp && p->ptr <= alloct);
}

/*
 * Realloc(p, nbytes) reallocates a block obtained from malloc() and freed
 * since last call of malloc() to have new size nbytes, and old content
 * returns new location, or 0 on failure.
 */
void *
realloc(vp, nbytes)
	register void *vp;
	size_t nbytes;
{
	register union store *p = vp;
	register union store *q;
	union store *s, *t;
	register unsigned nw;
	unsigned onw;

	if (p == NULL)
	        return malloc(nbytes);
	if (testbusy(p[-1].ptr))
		free((char *)p);
	onw = p[-1].ptr - p;
	q = (union store *)malloc(nbytes);
	if (q == NULL || q == p)
		return((char *)q);
	s = p;
	t = q;
	nw = (nbytes+WORD-1)/WORD;
	if (nw < onw)
		onw = nw;
	while (onw-- != 0)
		*t++ = *s++;
	if (q < p && q+nw >= p)
		(q+(q+nw-p))->ptr = allocx;
	return((char *)q);
}
