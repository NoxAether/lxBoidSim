SHELL := /bin/bash

CC := gcc
CFLAGS_MAIN := -I/usr/local/include -Wall -Wextra -O2
CFLAGS_DEBUG := -I/usr/local/include -O0 -g

LDFLAGS := -L/usr/local/lib -lraylib -lm -pthread

SRC_DIR := src

OBJ_DIR := obj
OBJ_DIR_DEB := obj_deb

BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ1 := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ2 := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_DEB)/%.o, $(SRC))

BIN1 := $(BIN_DIR)/boid_app
BIN2 := $(BIN_DIR)/boid_debug

all: $(BIN1) $(BIN2)

$(BIN1): $(OBJ1)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Should already have the BIN_DIR created.
$(BIN2): $(OBJ2)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS_MAIN)

$(OBJ_DIR_DEB)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR_DEB)
	$(CC) -c $< -o $@ $(CFLAGS_DEBUG)



run: $(BIN1)
	@./$(BIN1)

clean_obj:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR_DEB)

clean: clean_obj
	@rm -rf $(BIN_DIR)
