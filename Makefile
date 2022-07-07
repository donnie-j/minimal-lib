all:
	make -C src 

examples: dummy
	make -C examples 

dummy:

clean:
	make -C src clean
	make -C examples clean

