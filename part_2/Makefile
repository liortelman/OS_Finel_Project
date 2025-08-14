# Part 2 – build server/client (standalone) + optional Euler demo

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic -pthread
INCLUDES := -Iinclude
LDFLAGS :=

# ==== Server/Client (standalone, use Graph from part_2/include & src) ====
SERVER_SRC := server/server.cpp src/graph.cpp
CLIENT_SRC := client/client.cpp

server: $(SERVER_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o ../bin/server $(SERVER_SRC) $(LDFLAGS)

client: $(CLIENT_SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o ../bin/client $(CLIENT_SRC) $(LDFLAGS)

run-server: server
	../bin/server 5000

run-client: client
	../bin/client 127.0.0.1 5000

# ==== Optional: Euler demo (depends on euler.hpp; skip if missing) ====
# If you later add part_2/include/euler.hpp, you can build this:
EULER_DEMO := part_2_demo
EULER_SRC  := src/euler.cpp main_part2.cpp

euler_demo: $(EULER_SRC)
	@mkdir -p .
	$(CXX) $(CXXFLAGS) -Iinclude -I../part_1/include -o $(EULER_DEMO) $(EULER_SRC) ../part_1/src/graph.cpp

run_euler: euler_demo
	./$(EULER_DEMO)

clean:
	rm -f ../bin/server ../bin/client $(EULER_DEMO) \
	      server/*.o client/*.o src/*.o
