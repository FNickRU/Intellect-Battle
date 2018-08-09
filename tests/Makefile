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

TEST_SERV_SRC = server
TEST_SERV_OBJ = $(TEST_SERV_SRC:=.o)
TEST_SERV = $(patsubst %.o,test_%.out,$(TEST_SERV_OBJ))

TEST_CL_SRC = client_logic
TEST_CL_OBJ = $(TEST_CL_SRC:=.o)
TEST_CL = $(patsubst %.o,test_%.out,$(TEST_CL_OBJ))

TESTS = $(TEST_R) $(TEST_CL)

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

$(TEST_SERV): $(OBJ_DIR)/$(TEST_SERV_OBJ) $(TEST_SERV:.out=.o)
	$(LINK) $(OBJ)/$(TEST_SERV:.out=.o) $(OBJ_DIR)/$(TEST_SERV_OBJ) \
		$(LFLAGS) -o $(BIN_DIR)/$(TEST_SERV)

$(TEST_CL): $(OBJ_DIR)/$(TEST_CL_OBJ) $(TEST_CL:.out=.o)
	$(LINK) $(OBJ)/$(TEST_CL:.out=.o) $(OBJ_DIR)/$(TEST_CL_OBJ) \
		$(LFLAGS) -o $(BIN_DIR)/$(TEST_CL)

%.o: %.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@

clean:
	rm -rf *.o $(TESTS)

test_r: $(BIN_DIR)/$(TEST_R)
	./$(BIN_DIR)/$(TEST_R)]

test_srv: $(BIN_DIR)/$(TEST_SERV)
	./$(BIN_DIR)/$(TEST_SERV)

test_cl: $(BIN_DIR)/$(TEST_CL)
	./$(BIN_DIR)/$(TEST_CL)

run: test_r test_cl test_srv

deps:
	/bin/bash ../check.sh $(DEPENDECIES_LIST)
