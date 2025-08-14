# Part 2 – build server/client (standalone) + optional Euler demo

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic -pthread
INCLUDES := -Iinclude

# ==== Server/Client (standalone, use Graph from part_2/include & src) ====
SERVER_SRC := server/server.cpp src/graph.cpp
CLIENT_SRC := client/client.cpp

server: $(SERVER_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o ../bin/server $(SERVER_SRC)

client: $(CLIENT_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o ../bin/client $(CLIENT_SRC)

run-server: server
	../bin/server 5000

run-client: client
	../bin/client 127.0.0.1 5000

clean:
	rm -f ../bin/server ../bin/client
