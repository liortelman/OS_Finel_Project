#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

using namespace osproj;

Graph::Graph(size_t n, GraphType t) : _n(n), _type(t) {
    _adj.assign(_n, {});
    _indeg.assign(_n, 0);
    _outdeg.assign(_n, 0);
}

void Graph::resize(size_t n) {
    _n = n;
    _adj.assign(_n, {});
    _indeg.assign(_n, 0);
    _outdeg.assign(_n, 0);
    _edges.clear();
}

int Graph::add_vertex() {
    _adj.emplace_back();
    _indeg.push_back(0);
    _outdeg.push_back(0);
    return static_cast<int>(_n++);
}

void Graph::validate_vertex(int u) const {
    if (u < 0 || static_cast<size_t>(u) >= _n) {
        throw std::out_of_range("Vertex index out of range");
    }
}

void Graph::add_edge(int u, int v, double w) {
    validate_vertex(u);
    validate_vertex(v);

    const int eid = static_cast<int>(_edges.size());
    _edges.push_back({eid, u, v, w});

    // For UNDIRECTED: add both directions; for DIRECTED: only u->v
    _adj[u].push_back({v, eid, w});
    _outdeg[u]++;
    _indeg[v]++;

    if (_type == GraphType::UNDIRECTED) {
        _adj[v].push_back({u, eid, w});
        _outdeg[v]++;
        _indeg[u]++;
    }
}

const std::vector<Edge>& Graph::neighbors(int u) const {
    validate_vertex(u);
    return _adj[u];
}

int Graph::out_degree(int u) const {
    validate_vertex(u);
    return _outdeg[u];
}

int Graph::in_degree(int u) const {
    validate_vertex(u);
    return _indeg[u];
}

int Graph::degree(int u) const {
    validate_vertex(u);
    if (_type == GraphType::DIRECTED) return _outdeg[u];
    return _outdeg[u]; // for undirected outdeg==indeg==degree (we maintained both)
}

Graph Graph::from_file(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) throw std::runtime_error("Failed to open file: " + path);
    return from_stream(fin);
}

static GraphType parse_type(const std::string& s) {
    if (s.size() == 1) {
        char c = std::toupper(static_cast<unsigned char>(s[0]));
        if (c == 'U') return GraphType::UNDIRECTED;
        if (c == 'D') return GraphType::DIRECTED;
    }
    throw std::runtime_error("Invalid graph type (expected 'U' or 'D')");
}

Graph Graph::from_stream(std::istream& in) {
    // Format: TYPE N M  then M lines: u v [w]
    std::string t;
    size_t N, M;
    if (!(in >> t >> N >> M)) {
        throw std::runtime_error("Invalid header. Expected: TYPE N M");
    }
    GraphType gt = parse_type(t);
    Graph g(N, gt);

    for (size_t i = 0; i < M; ++i) {
        int u, v; double w = 1.0;
        if (!(in >> u >> v)) {
            throw std::runtime_error("Invalid edge line, expected: u v [w]");
        }
        if (in.peek() == '
' || in.peek() == '
') {
            // weight omitted
        } else {
            // try parse weight (if provided)
            std::streampos pos = in.tellg();
            if (!(in >> w)) {
                // revert if not a number
                in.clear();
                in.seekg(pos);
                w = 1.0;
            }
        }
        g.add_edge(u, v, w);
    }
    return g;
}

void Graph::to_stream(std::ostream& out) const {
    const char typ = (_type == GraphType::UNDIRECTED ? 'U' : 'D');
    out << typ << ' ' << _n << ' ' << _edges.size() << '
';
    for (const auto& e : _edges) {
        out << e.u << ' ' << e.v;
        if (e.w != 1.0) out << ' ' << e.w;
        out << '
';
    }
}
