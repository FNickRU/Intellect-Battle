#Build/Rebuild: make build
#Compile: make
#Run tests: make test

DEPENDECIES_LIST = "libcunit[1]-dev"

SRC_DIR = ../src
OBJ_DIR = ../obj

INC = ../include
OBJ = ../obj/test
MODULE_OBJ = ../obj
TEST_TEST = test_test.out
TEST_OBJ = ../obj/test.o
TESTS = $(TEST_TEST)

CC = gcc
CFLAGS = -std=gnu99 -I$(INC)

LINK = gcc
LFLAGS = -lcunit

.PHONY: all clean build run

all: deps $(TESTS) run

build: clean all

$(TEST_TEST): $(TEST_TEST:.out=.o)
	$(LINK) $(OBJ)/$(TEST_TEST:.out=.o) $(TEST_OBJ) $(LFLAGS) -o $(TEST_TEST)

%.o: %.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@

clean:
	rm -rf *.o $(TESTS)

run:
	./$(TEST_TEST)

deps:
	/bin/bash ../check.sh $(DEPENDECIES_LIST)
