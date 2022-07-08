OBJS = main.o led.o version.o

LD_FILE = ../../ld/sh_bare.x
LIBGCC = $(shell $(CC) -mj2 -print-libgcc-file-name)

CC       = sh2-elf-gcc
CXX      = sh2-elf-gcc
LD       = sh2-elf-ld
CFLAGS   = -mj2 -Os -g -Wall -I../../include
CXXFLAGS = -mj2 -Os -g -fno-exceptions -Wall -I../../include
LDFLAGS  = -T $(LD_FILE) -L../.. -lstdc++ -lc ../../crt0.o $(LIBGCC)

all: image.elf

image.elf: $(CRT_OBJS) $(OBJS)
	$(LD) $(OBJS) $(CRT_OBJS) -o $@ $(LDFLAGS)

version.c:
	@printf "char version_string[] = \"revision: $(shell hg head | head -1)\\\\nbuild: $(shell date)\\\\n\";\n" > $@

clean:
	- rm *.[oa] image.elf version.c

