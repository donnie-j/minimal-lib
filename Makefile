all:
	make -C src 

examples:
	make -C examples/calc 

clean:
	make -C src clean
	make -C examples/calc clean

