#Build/Rebuild: make build
#Compile: make
#Run tests: make test

SRC_DIR = ../src
OBJ_DIR = ../obj

INC = $(wildcard $(SRC_DIR)/*.h)
OBJ = $(INC:$(SRC_DIR)/%.h=$(OBJ_DIR)/%.o)
TEST_TEST = test_test.out
TESTS = $(TEST_TEST)

CC = gcc
CFLAGS = -std=gnu99 -I.

LINK = gcc
LFLAGS = -I. -lcunit

.PHONY: all clean build run

all: $(TESTS) run

build: clean all

$(TEST_TEST): $(TEST_TEST:.out=.o) $(OBJ)
	$(LINK) $(TEST_TEST:.out=.o) $(OBJ) $(LFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TESTS)

run:
	./$(TEST_TEST)
