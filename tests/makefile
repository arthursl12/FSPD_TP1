all: build

build: test_list.c
	gcc -Iunity -o test_list test_list.c

run: build
	./test_list $(ARGS)

clean: 
	/bin/rm -f *.o test_list
