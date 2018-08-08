#Build/Rebuild: make build
#Compile: make
#Run tests: make test

DEPENDENCIES_LIST:="ncurses gcc"

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
TST_DIR = tests
BIN_DIR = bin
DIRS = $(SRC_DIR) $(OBJ_DIR) $(TST_DIR) $(BIN_DIR)

SERV_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/server_main.c $(SRC_DIR)/room.c $(SRC_DIR)/worker.c $(SRC_DIR)/unit.c $(SRC_DIR)/unit_init.c
CLNT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/client_logic.c

SERV_OBJ = $(SERV_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CLNT_OBJ = $(CLNT_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SERV = server.out
CLNT = client.out

CC = gcc
CFLAGS = -std=gnu99

LINK = gcc
LFLAGS = -lpthread

.PHONY: all clean build test

all: deps $(DIRS) $(BIN_DIR)/$(CLNT) $(BIN_DIR)/$(SERV)

build: clean all

$(DIRS):
	mkdir -p $(DIRS)

$(BIN_DIR)/$(SERV): $(SERV_OBJ)
	$(LINK) $(SERV_OBJ) $(LFLAGS) -o $@

$(BIN_DIR)/$(CLNT): $(CLNT_OBJ)
	$(LINK) $(CLNT_OBJ) $(LFLAGS) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I./$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

test: all
	make -C $(TST_DIR)

deps:
	@echo "Checking dependencies"
	@/bin/bash ./check.sh $(DEPENDENCIES_LIST)
	@echo "All dependecies is fit!"
