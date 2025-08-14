#include "../include/graph.hpp"
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>

using namespace osproj;

// Returns Eulerian circuit or empty vector if none exists
std::vector<int> find_euler_circuit(const Graph& g) {
    if (g.directed()) {
        for (size_t i = 0; i < g.vertex_count(); ++i) {
            if (g.in_degree(i) != g.out_degree(i)) return {};
        }
    } else {
        for (size_t i = 0; i < g.vertex_count(); ++i) {
            if (g.degree(i) % 2 != 0) return {};
        }
    }

    std::unordered_map<int, std::multiset<int>> adj;
    for (size_t u = 0; u < g.vertex_count(); ++u) {
        for (auto& e : g.neighbors(u)) {
            adj[u].insert(e.to);
        }
    }

    std::vector<int> circuit;
    std::stack<int> st;
    st.push(0);

    while (!st.empty()) {
        int u = st.top();
        if (!adj[u].empty()) {
            int v = *adj[u].begin();
            adj[u].erase(adj[u].begin());
            if (!g.directed()) {
                adj[v].erase(adj[v].find(u));
            }
            st.push(v);
        } else {
            circuit.push_back(u);
            st.pop();
        }
    }

    for (const auto& [u, edges] : adj) {
        if (!edges.empty()) return {};
    }

    std::reverse(circuit.begin(), circuit.end());
    return circuit;
}
