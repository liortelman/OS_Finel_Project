#include "../include/graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>

extern std::vector<int> find_euler_circuit(const osproj::Graph&);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <graph_file>\n";
        return 1;
    }

    try {
        osproj::Graph g = osproj::Graph::from_file(argv[1]);

        std::vector<int> circuit = find_euler_circuit(g);

        if (circuit.empty()) {
            std::cout << "No Eulerian circuit exists.\n";
        } else {
            std::cout << "Eulerian circuit: ";
            for (int v : circuit)
                std::cout << v << ' ';
            std::cout << '\n';
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
