#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <limits>

namespace osproj {

enum class GraphType { DIRECTED, UNDIRECTED };

struct Edge {
    int to;
    int id;
    double w;
};

struct EdgeRecord {
    int id;
    int u, v;
    double w;
};

class Graph {
public:
    explicit Graph(size_t n = 0, GraphType t = GraphType::UNDIRECTED);

    void resize(size_t n);
    int add_vertex();
    void add_edge(int u, int v, double w = 1.0);

    size_t vertex_count() const { return _n; }
    size_t edge_count() const { return _edges.size(); }

    bool directed() const { return _type == GraphType::DIRECTED; }
    GraphType type() const { return _type; }

    const std::vector<Edge>& neighbors(int u) const;

    int out_degree(int u) const {
        validate_vertex(u);
        return _outdeg[u];
    }

    int in_degree(int u) const {
        validate_vertex(u);
        return _indeg[u];
    }

    int degree(int u) const {
        return directed() ? out_degree(u) : out_degree(u);
    }

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
    std::vector<std::vector<Edge>> _adj;
    std::vector<EdgeRecord> _edges;
    std::vector<int> _indeg, _outdeg;

    void validate_vertex(int u) const {
        if (u < 0 || static_cast<size_t>(u) >= _n)
            throw std::out_of_range("Invalid vertex: " + std::to_string(u));
    }
};

} // namespace osproj
