#Build/Rebuild: make build
#Compile: make
#Run tests: make test

SRC_DIR = src
OBJ_DIR = obj
TST_DIR = tests
BIN_DIR = bin
DIRS = $(SRC_DIR) $(OBJ_DIR) $(TST_DIR) $(BIN_DIR)

SRC = $(wildcard $(SRC_DIR)/*.c)
INC = $(wildcard $(SRC_DIR)/*.h)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN = a.out

CC = gcc
CFLAGS = -std=gnu99 -I.

LINK = gcc
LFLAGS = -I.

.PHONY: all clean build test

all: $(DIRS) $(BIN_DIR)/$(BIN)

build: clean all

$(DIRS):
	mkdir -p $(DIRS)

$(BIN_DIR)/$(BIN): $(OBJ)
	$(LINK) $(OBJ) $(LFLAGS) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN_DIR)/$(BIN)

test:
	make -C $(TST_DIR)
