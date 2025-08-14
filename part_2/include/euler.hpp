#pragma once
#include <vector>
#include <string>
#include <utility>
#include "../part_1/include/graph.hpp"  // Graph from part_1

namespace osproj {

struct EulerResult {
    bool has_circuit{false};
    std::vector<int> circuit;  // sequence of vertices (v0..vk), length = edges+1
    std::string reason;        // explanation when not eulerian
};

// Find Euler circuit for undirected or directed graph.
// If no circuit exists, has_circuit=false and 'reason' explains why.
EulerResult find_euler_circuit(const Graph& g);

} // namespace osproj
