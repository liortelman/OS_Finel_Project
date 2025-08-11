CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET := part_1_demo
SRC := main_part1.cpp
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
