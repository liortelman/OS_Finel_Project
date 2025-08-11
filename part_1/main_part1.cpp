#include "include/graph.hpp"
#include <iostream>
#include <sstream>

using namespace osproj;

// Small demo showcasing Part 1 (Graph) usage.
// Build & run (from repo root):
//   make
//   make run_part1
int main(int argc, char** argv) {
    try {
        // Demo 1: Build a small undirected graph in code
        Graph g1(5, GraphType::UNDIRECTED);
        g1.add_edge(0, 1);
        g1.add_edge(0, 2);
        g1.add_edge(1, 2);
        g1.add_edge(3, 4);

        std::cout << "[Demo] Undirected graph (U) with 5 vertices:" << std::endl;
        std::cout << g1 << std::endl; // prints in the defined format

        // Demo 2: Load from a text stream (same as loading from file)
        std::istringstream ss("U 4 4
0 1
1 2
2 3
3 0
");
        Graph g2 = Graph::from_stream(ss);

        std::cout << "[Demo] Degrees in g2:" << std::endl;
        for (int u = 0; u < 4; ++u) {
            std::cout << "u=" << u
                      << " deg=" << g2.degree(u)
                      << " out=" << g2.out_degree(u)
                      << " in=" << g2.in_degree(u) << '
';
        }

        // Demo 3: Directed example
        std::istringstream sd("D 3 3
0 1
1 2
2 1
");
        Graph g3 = Graph::from_stream(sd);
        std::cout << "
[Demo] Directed graph (D):" << std::endl;
        std::cout << g3 << std::endl;

        std::cout << "OK" << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
