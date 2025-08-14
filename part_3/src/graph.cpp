#include "graph.hpp"
#include <algorithm>

static const int INF = 1000000000;

void Graph::addNode(int id) {
    (void)adj[id]; // ensure adjacency entry exists (idempotent)
}

bool Graph::hasNode(int id) const {
    return adj.find(id) != adj.end();
}

bool Graph::addEdge(int u, int v, int w) {
    if (!hasNode(u) || !hasNode(v)) return false; // enforce existence
    adj[u].push_back({v, w});
    return true;
}

std::vector<int> Graph::bfs(int src) const {
    std::unordered_map<int, bool> vis;
    std::queue<int> q;
    std::vector<int> order;
    if (!hasNode(src)) return order; // empty if src missing
    q.push(src); vis[src] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        auto it = adj.find(u);
        if (it != adj.end()) {
            for (auto [v, w] : it->second) {
                (void)w;
                if (!vis[v]) { vis[v] = true; q.push(v); }
            }
        }
    }
    return order;
}

std::vector<int> Graph::dijkstra(int src, int maxNode) const {
    std::vector<int> dist(maxNode + 1, INF);
    if (!hasNode(src)) return dist;
    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        auto it = adj.find(u);
        if (it != adj.end()) {
            for (auto [v, w] : it->second) {
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }
    return dist;
}

std::optional<int> Graph::shortestPathUnweighted(int src, int dst) const {
    if (!hasNode(src) || !hasNode(dst)) return std::nullopt;
    std::unordered_map<int, int> dist;
    std::queue<int> q;
    q.push(src); dist[src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == dst) return dist[u];
        auto it = adj.find(u);
        if (it != adj.end()) {
            for (auto [v, w] : it->second) {
                (void)w;
                if (!dist.count(v)) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
    }
    return std::nullopt;
}
