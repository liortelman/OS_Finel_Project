#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <limits>

// =============================
// Graph data structure (Part 1)
// =============================
// Design goals:
// 1) Support both directed and undirected graphs.
// 2) Allow parallel edges (multi-graph) to keep Euler logic simple later.
// 3) Keep an edge list with stable IDs (useful for algorithms and debugging).
// 4) Provide simple I/O format for loading/saving graphs.
// 5) Keep degrees (in/out) for O(1) access.

namespace osproj {

enum class GraphType { DIRECTED, UNDIRECTED };

struct Edge {
    int to;        // destination vertex
    int id;        // global edge ID (refers to EdgeRecord index)
    double w;      // weight (default 1.0)
};

struct EdgeRecord {
    int id;        // global edge ID
    int u, v;      // endpoints as provided (for UNDIRECTED: stored once)
    double w;      // weight
};

class Graph {
public:
    // Construct an empty graph with n vertices.
    explicit Graph(size_t n = 0, GraphType t = GraphType::UNDIRECTED);

    // Resize (adds/removes vertices) — edges are cleared if shrinking below references.
    void resize(size_t n);

    // Add a vertex (returns its index)
    int add_vertex();

    // Add an edge (u, v) with weight w (default 1.0). Allows parallel edges.
    void add_edge(int u, int v, double w = 1.0);

    // Accessors
    size_t vertex_count() const { return _n; }
    size_t edge_count()   const { return _edges.size(); }

    bool   directed() const { return _type == GraphType::DIRECTED; }
    GraphType type()   const { return _type; }

    const std::vector<Edge>& neighbors(int u) const;

    // Degrees
    int out_degree(int u) const;     // for undirected == degree(u)
    int in_degree(int u)  const;     // for undirected == degree(u)
    int degree(int u)     const;     // undirected degree; for directed returns out_degree

    // I/O helpers (simple text format):
    // First line: TYPE N M        (TYPE in {U,D})
    // Next M lines: u v [w]
    // * Vertices are 0-based. * Weight is optional (default 1).
    static Graph from_file(const std::string& path);
    static Graph from_stream(std::istream& in);

    void to_stream(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const Graph& g) {
        g.to_stream(out);
        return out;
    }

private:
    size_t _n{};
    GraphType _type{GraphType::UNDIRECTED};
    std::vector<std::vector<Edge>> _adj;   // adjacency list
    std::vector<EdgeRecord> _edges;        // stable edge list
    std::vector<int> _indeg, _outdeg;      // degrees

    void validate_vertex(int u) const;
};

} // namespace osproj
