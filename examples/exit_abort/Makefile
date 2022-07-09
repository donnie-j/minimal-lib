AOBJS = main-abort.o
EOBJS = main-exit.o

LD_FILE = ../../ld/sh_bare.x
LIBGCC = $(shell $(CC) -mj2 -print-libgcc-file-name)

CC = sh2-elf-gcc
LD = sh2-elf-ld
CFLAGS = -mj2 -Os -g -Wall -I../../include
LDFLAGS = -T $(LD_FILE) -L../.. -lc ../../crt0.o $(LIBGCC)

all: abort.elf exit.elf

abort.elf: $(CRT_OBJS) $(AOBJS)
	$(LD) $(AOBJS) $(CRT_OBJS) -o $@ $(LDFLAGS)

exit.elf: $(CRT_OBJS) $(EOBJS)
	$(LD) $(EOBJS) $(CRT_OBJS) -o $@ $(LDFLAGS)

version.c:
	@printf "char version_string[] = \"revision: $(shell hg head | head -1)\\\\nbuild: $(shell date)\\\\n\";\n" > $@

clean:
	- rm *.[oa] *.elf version.c

