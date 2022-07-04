CC              = sh2-elf-gcc
AR              = sh2-elf-ar
RANLIB          = sh2-elf-ranlib
CFLAGS          += -I../../include -Os -Werror -Wall

OBJS            = asin.o atan.o exp.o erf.o floor.o fmod.o hypot.o j0.o j1.o \
                  jn.o log.o pow.o sin.o sinh.o sqrt.o tan.o tanh.o

all:            ../../libm.a

../../libm.a:	${OBJS}
		$(AR) cr $@ ${OBJS}
		$(RANLIB) $@

install:	all

clean:
		rm -f ../../libm.a *.o *~ tags

