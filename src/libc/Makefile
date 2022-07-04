CC              = sh2-elf-gcc
AR              = sh2-elf-ar
RANLIB          = sh2-elf-ranlib
CFLAGS          += -I../../include -Os -Werror -Wall

OBJS            = abs.o atof.o atol.o bcmp.o bcopy.o bzero.o ctype_.o \
                  fabs.o isinf.o isinff.o isnan.o isnanf.o malloc.o memccpy.o \
                  memchr.o memcmp.o memcpy.o memset.o rand.o brk.o \
                  sleep.o strcasecmp.o strcat.o strchr.o strcmp.o strcpy.o \
                  strdup.o strlen.o strncat.o strncmp.o strncpy.o strrchr.o \
                  strstr.o strtok.o swab.o usleep.o xprintf.o

all:            ../../libc.a

../../libc.a:	${OBJS}
		$(AR) cr $@ ${OBJS}
		$(RANLIB) $@

install:	all

clean:
		rm -f ../../libc.a *.o *~ tags
