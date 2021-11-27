all: build

build: mandel-tiles-graphic.c
	# gcc -o prog mandel-tiles-graphic.c -lgraph
	gcc -o prog mandel-tiles-graphic.c


run: build
	./prog $(ARGS)

clean: 
	/bin/rm -f *.o prog
