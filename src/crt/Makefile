CC              = sh2-elf-gcc
AR              = sh2-elf-ar
RANLIB          = sh2-elf-ranlib
CFLAGS          += -I../../include -Os -Werror -Wall -Wno-error=attributes

OBJS            = crt0.o

all:            ../../crt0.o

../../crt0.o:	${OBJS}
		cp $< $@

install:	all

clean:
		rm -f ../../crt0.o *.o *~ tags
