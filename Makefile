all:
	make -C src 
	make -C examples/calc 

clean:
	make -C src clean
	make -C examples/calc clean

