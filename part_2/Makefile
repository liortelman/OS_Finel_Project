# Part 2 – build server/client

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic -pthread
INCLUDES := -Iinclude

SERVER_SRC := server/server.cpp src/graph.cpp
CLIENT_SRC := client/client.cpp

SERVER_BIN := ../bin/server
CLIENT_BIN := ../bin/client

.PHONY: all clean run-server run-client server client
all: $(SERVER_BIN) $(CLIENT_BIN)

# build outputs are the real targets:
$(SERVER_BIN): $(SERVER_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SERVER_SRC)

$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(CLIENT_SRC)

# convenience aliases (phony so they won't clash with folders):
server: $(SERVER_BIN)
client: $(CLIENT_BIN)

run-server: $(SERVER_BIN)
	$(SERVER_BIN) 5000

run-client: $(CLIENT_BIN)
	$(CLIENT_BIN) 127.0.0.1 5000

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
