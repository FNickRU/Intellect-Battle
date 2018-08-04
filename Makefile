#Build/Rebuild: make build
#Compile: make
#Run tests: make test

DEPENDENCIES_LIST:="ncurses gcc libcunit[1]-dev"

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
TST_DIR = tests
BIN_DIR = bin
DIRS = $(SRC_DIR) $(OBJ_DIR) $(TST_DIR) $(BIN_DIR)

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN = result.out

CC = gcc
CFLAGS = -std=gnu99

LINK = gcc
LFLAGS =

.PHONY: all clean build test

all: deps $(DIRS) $(BIN_DIR)/$(BIN)

build: clean all

$(DIRS):
	mkdir -p $(DIRS)

$(BIN_DIR)/$(BIN): $(OBJ)
	$(LINK) $(OBJ) $(LFLAGS) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I./$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

test:
	make -C $(TST_DIR)

deps:
	@echo "Checking dependencies"
	@/bin/bash ./check.sh $(DEPENDENCIES_LIST)
	@echo "All dependecies is fit!"
