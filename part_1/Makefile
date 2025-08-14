# Makefile for part_1 — Graph structure + Euler algorithm

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

SOURCES = $(SRC_DIR)/graph.cpp $(SRC_DIR)/euler.cpp $(SRC_DIR)/main.cpp
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
EXEC = $(BIN_DIR)/euler

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	@echo "Usage: make run FILE=path/to/graph.txt"
	@./$(EXEC) $(FILE)
