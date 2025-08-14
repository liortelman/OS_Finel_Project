#include "euler.hpp"
#include <stack>
#include <algorithm>
#include <queue>
#include <unordered_set>

namespace osproj {

static bool has_edges(const Graph& g) {
    return g.edge_count() > 0;
}

// ---------- Undirected: connectivity among vertices with deg>0 ----------
static bool connected_ignoring_zero_deg_undirected(const Graph& g) {
    const int n = static_cast<int>(g.vertex_count());
    int start = -1;
    for (int u = 0; u < n; ++u) if (g.degree(u) > 0) { start = u; break; }
    if (start == -1) return true; // no edges -> trivially connected for Euler considerations

    std::vector<char> vis(n, 0);
    std::stack<int> st;
    st.push(start); vis[start] = 1;
    int seen = 0;

    while (!st.empty()) {
        int u = st.top(); st.pop(); ++seen;
        for (const auto& e : g.neighbors(u)) {
            if (!vis[e.to] && g.degree(e.to) > 0) {
                vis[e.to] = 1;
                st.push(e.to);
            }
        }
    }

    for (int u = 0; u < n; ++u)
        if (g.degree(u) > 0 && !vis[u]) return false;
    return true;
}

// ---------- Directed: strong connectivity on vertices with in+out>0 ----------
static bool strongly_connected_on_nonzero_deg_directed(const Graph& g) {
    const int n = static_cast<int>(g.vertex_count());

    // Build reverse adjacency (only for directed)
    std::vector<std::vector<int>> radj(n);
    int start = -1, active = 0;

    for (int u = 0; u < n; ++u) {
        int deg = g.out_degree(u) + g.in_degree(u);
        if (deg > 0) { ++active; if (start == -1) start = u; }
        for (const auto& e : g.neighbors(u)) {
            radj[e.to].push_back(u);
        }
    }
    if (active == 0) return true; // no edges

    // DFS forward
    std::vector<char> vis(n, 0);
    std::stack<int> st; st.push(start); vis[start] = 1;
    int seen = 0;
    while (!st.empty()) {
        int u = st.top(); st.pop(); ++seen;
        for (const auto& e : g.neighbors(u)) {
            if (!vis[e.to] && (g.out_degree(e.to)+g.in_degree(e.to) > 0)) {
                vis[e.to] = 1; st.push(e.to);
            }
        }
    }
    for (int u = 0; u < n; ++u)
        if ((g.out_degree(u)+g.in_degree(u) > 0) && !vis[u]) return false;

    // DFS backward (on reverse graph)
    std::fill(vis.begin(), vis.end(), 0);
    st.push(start); vis[start] = 1; seen = 0;
    while (!st.empty()) {
        int u = st.top(); st.pop(); ++seen;
        for (int v : radj[u]) {
            if (!vis[v] && (g.out_degree(v)+g.in_degree(v) > 0)) {
                vis[v] = 1; st.push(v);
            }
        }
    }
    for (int u = 0; u < n; ++u)
        if ((g.out_degree(u)+g.in_degree(u) > 0) && !vis[u]) return false;

    return true;
}

// ---------- Preconditions ----------
static bool eulerian_undirected_ok(const Graph& g, std::string& why_not) {
    if (!connected_ignoring_zero_deg_undirected(g)) {
        why_not = "Graph is not connected when ignoring zero-degree vertices";
        return false;
    }
    const int n = static_cast<int>(g.vertex_count());
    for (int u = 0; u < n; ++u) {
        if (g.degree(u) % 2 != 0) {
            why_not = "A vertex has odd degree (undirected Euler circuit requires all degrees even)";
            return false;
        }
    }
    return true;
}

static bool eulerian_directed_ok(const Graph& g, std::string& why_not) {
    const int n = static_cast<int>(g.vertex_count());
    for (int u = 0; u < n; ++u) {
        if (g.in_degree(u) != g.out_degree(u)) {
            why_not = "in-degree != out-degree for some vertex (directed Euler circuit requires equality)";
            return false;
        }
    }
    if (!strongly_connected_on_nonzero_deg_directed(g)) {
        why_not = "Directed graph is not strongly connected over nonzero-degree vertices";
        return false;
    }
    return true;
}

// ---------- Hierholzer (works for both directed/undirected) ----------
static std::vector<int> hierholzer_circuit(const Graph& g) {
    const int n = static_cast<int>(g.vertex_count());
    std::vector<std::vector<Edge>> adj_copy(n);
    for (int u = 0; u < n; ++u) adj_copy[u] = g.neighbors(u);

    // Track used edges by global edge id
    std::vector<char> used(g.edge_count(), 0);

    auto nonzero_start = [&]() -> int {
        for (int u = 0; u < n; ++u)
            if (g.out_degree(u) + g.in_degree(u) > 0) return u;
        return 0; // no edges -> 0 if exists
    };
    int start = nonzero_start();

    std::vector<int> idx(n, 0);
    std::vector<int> circuit;
    std::vector<int> st; st.push_back(start);

    while (!st.empty()) {
        int u = st.back();
        // advance to next unused edge
        while (idx[u] < static_cast<int>(adj_copy[u].size()) &&
               used[ adj_copy[u][idx[u]].id ]) {
            ++idx[u];
        }
        if (idx[u] == static_cast<int>(adj_copy[u].size())) {
            // no edges left from u
            circuit.push_back(u);
            st.pop_back();
        } else {
            const Edge e = adj_copy[u][ idx[u]++ ];
            if (!used[e.id]) {
                used[e.id] = 1;
                st.push_back(e.to);
            }
        }
    }

    // circuit is built in reverse, but that's fine for eulerian circuit semantics.
    // Optional: reverse(circuit.begin(), circuit.end());
    return circuit;
}

EulerResult find_euler_circuit(const Graph& g) {
    EulerResult res;

    // No edges -> define trivial circuit (single vertex if any)
    if (!has_edges(g)) {
        res.has_circuit = true;
        if (g.vertex_count() > 0) res.circuit.push_back(0);
        return res;
    }

    std::string why_not;
    if (g.type() == GraphType::UNDIRECTED) {
        if (!eulerian_undirected_ok(g, why_not)) {
            res.has_circuit = false;
            res.reason = why_not;
            return res;
        }
    } else {
        if (!eulerian_directed_ok(g, why_not)) {
            res.has_circuit = false;
            res.reason = why_not;
            return res;
        }
    }

    // Build the circuit
    std::vector<int> path = hierholzer_circuit(g);

    // Validate size: must use all edges exactly once
    // For undirected we stored each edge id once; for directed also once.
    if (path.size() != g.edge_count() + 1) {
        res.has_circuit = false;
        res.reason = "Failed to traverse all edges exactly once (internal)";
        return res;
    }

    res.has_circuit = true;
    res.circuit = std::move(path);
    return res;
}

} // namespace osproj
