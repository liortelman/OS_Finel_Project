// graph.hpp
#pragma once
#include <unordered_map>
#include <vector>
#include <optional>
#include <queue>
#include <limits>
#include <utility>

class Graph {
public:
    // Adds node if it does not exist
    void addNode(int id);
    // Adds directed edge u->v with weight w (use w=1 for unweighted)
    void addEdge(int u, int v, int w = 1);

    // BFS visit order from src
    std::vector<int> bfs(int src) const;

    // Dijkstra distances from src (size up to maxNode), INF for unreachable
    std::vector<int> dijkstra(int src, int maxNode) const;

    // Unweighted shortest path length (in hops)
    std::optional<int> shortestPathUnweighted(int src, int dst) const;

private:
    // adjacency list: u -> vector of (v, w)
    std::unordered_map<int, std::vector<std::pair<int,int>>> adj;
};
