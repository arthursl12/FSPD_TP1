# Using µnit is very simple; just include the header and add the C
# file to your sources.  That said, here is a simple Makefile to build
# the example.

CC := gcc
INCLUDES := -I.. -Imunit 
BIN_FOLDER := bin/

FRAMEWORK_FOLDER := munit/
FRAMEWORK_MODULE := $(FRAMEWORK_FOLDER)/munit.c

MODULES := ../linked_list.c
TEST_MODULES := test_linked_list.c

build: $(FRAMEWORK_MODULE) $(MODULES)
	mkdir $(BIN_FOLDER)
	rm -f test_suite
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_FOLDER)test_suite $(FRAMEWORK_FOLDER)munit.c $(TEST_MODULES) $(MODULES)

test:
	$(TEST_ENV) ./$(BIN_FOLDER)test_suite

clean:
	rm -f $(BIN_FOLDER)test_suite

all: build
