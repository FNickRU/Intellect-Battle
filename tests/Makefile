#Build/Rebuild: make build
#Compile: make
#Run tests: make test

DEPENDECIES_LIST = "libcunit[1]-dev"

SRC_DIR = ../src
OBJ_DIR = ../obj

BIN_DIR = ../bin

INC = ../include
OBJ = ../obj/test

TEST_R_SRC = room
TEST_R_OBJ = $(TEST_R_SRC:=.o)
TEST_R = $(patsubst %.o,test_%.out,$(TEST_R_OBJ))

TESTS = $(TEST_R)

CC = gcc
CFLAGS = -std=gnu99 -I$(INC)

LINK = gcc
LFLAGS = -lcunit

.PHONY: all clean build run

all: deps $(TESTS) run

build: clean all

$(TEST_R): $(OBJ_DIR)/$(TEST_R_OBJ) $(TEST_R:.out=.o)
	$(LINK) $(OBJ)/$(TEST_R:.out=.o) $(OBJ_DIR)/$(TEST_R_OBJ) \
		$(LFLAGS) -o $(BIN_DIR)/$(TEST_R)

%.o: %.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@

clean:
	rm -rf *.o $(TESTS)


run: test_r


test_r: $(BIN_DIR)/$(TEST_R)
	./$(BIN_DIR)/$(TEST_R)


deps:
	/bin/bash ../check.sh $(DEPENDECIES_LIST)
