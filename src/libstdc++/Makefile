CXX             = sh2-elf-g++
AR              = sh2-elf-ar
RANLIB          = sh2-elf-ranlib
CXXFLAGS        += -mj2 -I../../include -Os -Werror -Wall

OBJS            = new.o delete.o cxa_pure_virtual.o

all:            ../../libstdc++.a

../../libstdc++.a:	${OBJS}
		$(AR) cr $@ ${OBJS}
		$(RANLIB) $@

install:	all

clean:
		rm -f ../../libstdc++.a *.o *~ tags

