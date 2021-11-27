BIN_DIR := bin/

all: build

build: bin | mandel-tiles-graphic.c
	# gcc -o prog mandel-tiles-graphic.c -lgraph
	g++ -o $(BIN_DIR)prog mandel-tiles-graphic.c

bin: 
	mkdir $(BIN_DIR)
	
run: build
	./$(BIN_DIR)prog $(ARGS)

clean: 
	/bin/rm -f *.o prog
