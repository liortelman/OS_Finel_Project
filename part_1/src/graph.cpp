#include "../include/graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace osproj;

// Constructor
Graph::Graph(size_t n, GraphType t) : _n(n), _type(t), _adj(n), _indeg(n, 0), _outdeg(n, 0) {}

// Resize
void Graph::resize(size_t n) {
    _n = n;
    _adj.resize(n);
    _indeg.resize(n, 0);
    _outdeg.resize(n, 0);
    _edges.clear();
}

// Add vertex
int Graph::add_vertex() {
    _adj.emplace_back();
    _indeg.push_back(0);
    _outdeg.push_back(0);
    return _n++;
}

// Add edge
void Graph::add_edge(int u, int v, double w) {
    validate_vertex(u);
    validate_vertex(v);
    int id = static_cast<int>(_edges.size());
    _edges.push_back({id, u, v, w});
    _adj[u].push_back({v, id, w});
    _outdeg[u]++;
    _indeg[v]++;
    if (_type == GraphType::UNDIRECTED && u != v) {
        _adj[v].push_back({u, id, w});
        _outdeg[v]++;
        _indeg[u]++;
    }
}

// Load from file
Graph Graph::from_file(const std::string& path) {
    std::ifstream fin(path);
    if (!fin)
        throw std::runtime_error("Failed to open file: " + path);
    return from_stream(fin);
}

// Load from stream
Graph Graph::from_stream(std::istream& in) {
    char typ;
    size_t n, m;
    if (!(in >> typ >> n >> m))
        throw std::runtime_error("Invalid header line (expected: TYPE N M)");

    Graph g(n, typ == 'D' ? GraphType::DIRECTED : GraphType::UNDIRECTED);
    for (size_t i = 0; i < m; ++i) {
        int u, v;
        double w = 1.0;
        if (!(in >> u >> v)) throw std::runtime_error("Invalid edge line");
        if (in.peek() == ' ' || in.peek() == '\t') {
            if (!(in >> w)) w = 1.0;
        }
        g.add_edge(u, v, w);
    }
    return g;
}

// Write to stream
void Graph::to_stream(std::ostream& out) const {
    out << (directed() ? 'D' : 'U') << ' ' << _n << ' ' << _edges.size() << '\n';
    for (const auto& e : _edges) {
        out << e.u << ' ' << e.v;
        if (e.w != 1.0)
            out << ' ' << e.w;
        out << '\n';
    }
}
