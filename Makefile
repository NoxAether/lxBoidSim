SHELL := /bin/bash

CC := gcc
CFLAGS := -I/usr/local/include -Wall -Wextra -O2
LDFLAGS := -L/usr/local/lib -lraylib -lm

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
BIN := $(BIN_DIR)/boid_sim

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(BIN)
	@./$(BIN)

clean_obj:
	@rm -rf $(OBJ_DIR)

clean: clean_obj
	@rm -rf $(BIN_DIR)
