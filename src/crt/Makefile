CC              = sh2-elf-gcc
AR              = sh2-elf-ar
RANLIB          = sh2-elf-ranlib
CFLAGS          += -I../../include -Os -Werror -Wall -Wno-error=attributes

OBJS            = crt0.o crt1.o

all:	${OBJS}
		cp ${OBJS} ../../

install:	all

clean:
		rm -f ../../crt?.o *.o *~ tags
