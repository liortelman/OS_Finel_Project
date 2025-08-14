# Part 3 – Thread Pool server/client
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic -pthread
INCLUDES := -Iinclude

SERVER_SRC := server/main.cpp src/graph.cpp
CLIENT_SRC := client/main.cpp

SERVER_BIN := ../bin/part3_server
CLIENT_BIN := ../bin/part3_client

PORT ?= 5000

.PHONY: all clean server client run-server run-client
all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SERVER_SRC)

$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(CLIENT_SRC)

server: $(SERVER_BIN)
client: $(CLIENT_BIN)

run-server: $(SERVER_BIN)
	$(SERVER_BIN) $(PORT)

run-client: $(CLIENT_BIN)
	$(CLIENT_BIN) 127.0.0.1 $(PORT)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
