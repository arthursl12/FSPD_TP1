BIN_DIR := bin/

all: build

build: mandel-tiles-graphic.c
	mkdir $(BIN_DIR)
	# gcc -o prog mandel-tiles-graphic.c -lgraph
	gcc -o $(BIN_DIR)prog mandel-tiles-graphic.c


run: build
	./$(BIN_DIR)prog $(ARGS)

clean: 
	/bin/rm -f *.o prog
