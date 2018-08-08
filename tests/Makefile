#Build/Rebuild: make build
#Compile: make
#Run tests: make test

DEPENDECIES_LIST = "libcunit[1]-dev"

SRC_DIR = ../src
OBJ_DIR = ../obj

INC = ../include
OBJ = ../obj/test
TEST_CL = test_client_logic.out
TEST_CLC = $(patsubst %.out,%.c,$(TEST_CL))
TESTS = $(TEST_CL)

CC = gcc
CFLAGS = -std=gnu99 -I$(INC)

LINK = gcc
LFLAGS = -lcunit

.PHONY: all clean build run

all: deps $(TESTS) run

build: clean all

$(TEST_CL): $(TEST_CLC)
	$(LINK) $(CFLAGS) $(TEST_CLC) $(OBJ_DIR)/client_logic.o \
		$(LFLAGS) -o $(TEST_CL)

%.o: %.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@

clean:
	rm -rf *.o $(TESTS)

#run:

deps:
	/bin/bash ../check.sh $(DEPENDECIES_LIST)
