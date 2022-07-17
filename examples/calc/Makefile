OBJS = main-lcd.o lcd-drv.o rpn.o version.o

SPECS_FILE = specs.12
CC = sh2-elf-gcc
CFLAGS = -mj2 -Os -g -Wall -I../../include
LDFLAGS = -mcu=12k -L../.. -lm -lc

all: image.elf

image.elf: $(CRT_OBJS) $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -specs=$(SPECS_FILE) -o $@

version.c:
	@printf "char version_string[] = \"revision: $(shell hg head | head -1)\\\\nbuild: $(shell date)\\\\n\";\n" > $@

clean:
	- rm *.[oa] image.elf version.c

