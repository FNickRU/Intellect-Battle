#Build/Rebuild: make build
#Compile: make
#Run tests: make test

DEPENDECIES_LIST = "libcunit[1]-dev"

SRC_DIR = ../src
OBJ_DIR = ../obj

INC = ../include
OBJ = ../obj/test
TESTS =

CC = gcc
CFLAGS = -std=gnu99 -I$(INC)

LINK = gcc
LFLAGS = -lcunit

.PHONY: all clean build run

all: deps $(TESTS) run

build: clean all

%.o: %.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@

clean:
	rm -rf *.o $(TESTS)

#run:

deps:
	/bin/bash ../check.sh $(DEPENDECIES_LIST)
