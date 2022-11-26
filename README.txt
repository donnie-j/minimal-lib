A minimal libc for bare metal systems.

What is this?

This minimal library and C runtime was written to support bare metal J-Core
(and SH2) systems with very little memory.It assumes a standard sh2-elf GCC
compiler, patched for J-Core.  See the J-Core builder scripts for an example
of how to build such a compiler.

License

Most of the code here is repurposed from RetroBSD, and carries their 3 clause
BSD license.  In other words, it is suitable for deployement in OpenSource
and closed embedded systems.  The example code is free for you to use in any
way you like.  See LICENSE.txt

Building and Using

Running 'make' builds 3 and the C Runtime startup files in this directory:
libc.a libm.a libstdc++.a and crt0.o

Make sure to use -I<path to>./include when compiling, and -L<path to>. when
linking.

Running 'make examples' will build a small RPN calculator application, which
shows how to use a 'specs' file to manage linking.  This will run on an iCE40
hardware platform, (ToDo: make open hardware).  It will also build small test
ROMs for C++, atexit, exit/abort, and setjmp/longjmp.  These will run on a
J-Core CPU in e.g. an RTL simulator (ToDo: provide makefile or scripts)

Porting

This code is not specifically hardwired for J-Core or SH2.  However, there is
not (yet) any target specific segmentation of the parts that are architecture
specific.  At least crt0.S, setjmp and longjmp will need to be implemented for
your architecture, and the linker scripts will need to be changed for the
memory map of your target hardware.

