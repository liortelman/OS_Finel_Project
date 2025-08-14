#include "include/euler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace osproj;

// Build & run from repo root:
//   make part_2
//   make run_part2
// Or: ./part_2/part_2_demo <optional_graph_file>

int main(int argc, char** argv) {
    try {
        Graph g;

        if (argc > 1) {
            // Load from file provided as first arg
            std::string path = argv[1];
            g = Graph::from_file(path);
            std::cout << "[Loaded] " << path << "\n";
        } else {
            // Fallback demo (undirected 0-1-2-0 Eulerian cycle)
            std::istringstream ss("U 3 3\n0 1\n1 2\n2 0\n");
            g = Graph::from_stream(ss);
            std::cout << "[Demo] Using a small undirected Eulerian triangle\n";
        }

        EulerResult r = find_euler_circuit(g);
        if (!r.has_circuit) {
            std::cout << "Euler circuit: NOT FOUND\nReason: " << r.reason << "\n";
            return 0;
        }

        std::cout << "Euler circuit found. Vertex sequence (length=" << r.circuit.size() << "):\n";
        for (size_t i = 0; i < r.circuit.size(); ++i) {
            std::cout << r.circuit[i] << (i + 1 < r.circuit.size() ? " -> " : "\n");
        }
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}
