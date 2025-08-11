CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
INCLUDES := -Iinclude
LDFLAGS :=

TARGET := part_1_demo
SRC := src/graph.cpp main_part1.cpp
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp include/graph.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
