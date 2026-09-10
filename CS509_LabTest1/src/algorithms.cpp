#include "algorithms.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <stack>
#include <utility>

BFSResult bfs(const CSRGraph& graph, int source) {
    BFSResult result;
    result.distance.assign(graph.V, -1);

    if (source < 0 || source >= graph.V) return result;

    std::queue<int> q;
    result.distance[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.traversal.push_back(u);

        for (int p = graph.row_ptr[u]; p < graph.row_ptr[u + 1]; ++p) {
            int v = graph.col_idx[p];
            if (result.distance[v] == -1) {
                result.distance[v] = result.distance[u] + 1;
                q.push(v);
            }
        }
    }
    return result;
}

std::vector<long long> sssp(const CSRGraph& graph, int source, bool weighted) {
    std::vector<long long> distance(graph.V, INF);
    if (source < 0 || source >= graph.V) return distance;

    // The original Assignment-1 implementation was Dijkstra.  For the
    // BFS<->SSSP validation, the same implementation is used with every edge
    // assigned unit cost, as required by the question.
    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du != distance[u]) continue;

        for (int p = graph.row_ptr[u]; p < graph.row_ptr[u + 1]; ++p) {
            int v = graph.col_idx[p];
            long long w = weighted ? graph.values[p] : 1LL;
            long long nd = du + w;
            if (nd < distance[v]) {
                distance[v] = nd;
                pq.push({nd, v});
            }
        }
    }
    return distance;
}

std::vector<int> connectedComponents(const CSRGraph& graph, int& componentCount) {
    std::vector<int> component(graph.V, -1);
    componentCount = 0;
    std::queue<int> q;

    for (int start = 0; start < graph.V; ++start) {
        if (component[start] != -1) continue;
        component[start] = componentCount;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int p = graph.row_ptr[u]; p < graph.row_ptr[u + 1]; ++p) {
                int v = graph.col_idx[p];
                if (component[v] == -1) {
                    component[v] = componentCount;
                    q.push(v);
                }
            }
        }
        ++componentCount;
    }
    return component;
}

TriangleResult triangleCounting(const CSRGraph& graph, bool listTriangles) {
    TriangleResult result;
    const int V = graph.V;

    // Same sorted-CSR intersection idea as the previous assignment.  Sorting
    // only the (u,v) CSR pairs avoids an adjacency-list conversion.
    std::vector<std::vector<std::pair<int,long long>>> rows(V);
    for (int u = 0; u < V; ++u) {
        rows[u].reserve(graph.row_ptr[u + 1] - graph.row_ptr[u]);
        for (int p = graph.row_ptr[u]; p < graph.row_ptr[u + 1]; ++p)
            rows[u].push_back({graph.col_idx[p], graph.values[p]});
        std::sort(rows[u].begin(), rows[u].end());
    }

    for (int u = 0; u < V; ++u) {
        for (const auto& [v, ignored1] : rows[u]) {
            if (v <= u) continue;

            size_t i = 0, j = 0;
            while (i < rows[u].size() && j < rows[v].size()) {
                int a = rows[u][i].first;
                int b = rows[v][j].first;
                if (a == b) {
                    if (a > v) {
                        ++result.count;
                        if (listTriangles)
                            result.triangles.emplace_back(u, v, a);
                    }
                    ++i;
                    ++j;
                } else if (a < b) {
                    ++i;
                } else {
                    ++j;
                }
            }
        }
    }
    return result;
}

std::vector<double> betweennessCentrality(
    const CSRGraph& graph,
    const std::vector<int>& component,
    int componentId
) {
    const int V = graph.V;
    std::vector<double> CB(V, 0.0);

    auto allowed = [&](int v) {
        return componentId < 0 || component[v] == componentId;
    };

    for (int s = 0; s < V; ++s) {
        if (!allowed(s)) continue;

        std::vector<std::vector<int>> P(V);
        std::vector<int> distance(V, -1);
        std::vector<double> sigma(V, 0.0);
        std::vector<int> S;
        std::queue<int> Q;

        distance[s] = 0;
        sigma[s] = 1.0;
        Q.push(s);

        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            S.push_back(v);

            for (int p = graph.row_ptr[v]; p < graph.row_ptr[v + 1]; ++p) {
                int w = graph.col_idx[p];
                if (!allowed(w)) continue;

                if (distance[w] < 0) {
                    distance[w] = distance[v] + 1;
                    Q.push(w);
                }
                if (distance[w] == distance[v] + 1) {
                    sigma[w] += sigma[v];
                    P[w].push_back(v);
                }
            }
        }

        std::vector<double> delta(V, 0.0);
        while (!S.empty()) {
            int w = S.back();
            S.pop_back();
            for (int v : P[w]) {
                if (sigma[w] != 0.0)
                    delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            }
            if (w != s) CB[w] += delta[w];
        }
    }

    // This is the raw, unnormalized undirected Brandes value, matching the
    // previous assignment's convention.
    for (int v = 0; v < V; ++v) {
        if (allowed(v)) CB[v] /= 2.0;
    }
    return CB;
}

std::vector<std::vector<long long>> initialDistanceMatrix(const CSRGraph& graph) {
    std::vector<std::vector<long long>> dist(
        graph.V, std::vector<long long>(graph.V, INF));

    for (int i = 0; i < graph.V; ++i) dist[i][i] = 0;

    for (int u = 0; u < graph.V; ++u) {
        for (int p = graph.row_ptr[u]; p < graph.row_ptr[u + 1]; ++p) {
            int v = graph.col_idx[p];
            dist[u][v] = std::min(dist[u][v], graph.values[p]);
        }
    }
    return dist;
}

static inline void relaxCell(long long& dij, long long dik, long long dkj) {
    if (dik == INF || dkj == INF) return;
    // The assignment's INF is sufficiently far from overflow for ordinary
    // test data, but this guard keeps INF arithmetic well-defined.
    if (dik > 0 && dkj > INF - dik) return;
    if (dik < 0 && dkj < -INF - dik) return;
    long long candidate = dik + dkj;
    if (candidate < dij) dij = candidate;
}

bool floydWarshall(std::vector<std::vector<long long>>& dist) {
    const int V = static_cast<int>(dist.size());
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < V; ++j) {
                relaxCell(dist[i][j], dist[i][k], dist[k][j]);
            }
        }
    }

    for (int i = 0; i < V; ++i)
        if (dist[i][i] < 0) return true;
    return false;
}

bool blockedFloydWarshall(std::vector<std::vector<long long>>& dist, int B) {
    const int V = static_cast<int>(dist.size());
    if (B <= 0) return false;

    // Standard blocked Floyd-Warshall: for each k-block, update
    //   1) diagonal block, 2) k-row/k-column blocks, 3) remaining blocks.
    // min(B, V) boundaries make V%B != 0 safe.
    for (int kk = 0; kk < V; kk += B) {
        const int kend = std::min(kk + B, V);

        // Phase 1: diagonal block.
        for (int k = kk; k < kend; ++k)
            for (int i = kk; i < kend; ++i)
                if (dist[i][k] != INF)
                    for (int j = kk; j < kend; ++j)
                        relaxCell(dist[i][j], dist[i][k], dist[k][j]);

        // Phase 2a: blocks in the same block row as the diagonal block.
        for (int jj = 0; jj < V; jj += B) {
            if (jj == kk) continue;
            const int jend = std::min(jj + B, V);
            for (int k = kk; k < kend; ++k)
                for (int i = kk; i < kend; ++i)
                    if (dist[i][k] != INF)
                        for (int j = jj; j < jend; ++j)
                            relaxCell(dist[i][j], dist[i][k], dist[k][j]);
        }

        // Phase 2b: blocks in the same block column as the diagonal block.
        for (int ii = 0; ii < V; ii += B) {
            if (ii == kk) continue;
            const int iend = std::min(ii + B, V);
            for (int k = kk; k < kend; ++k)
                for (int i = ii; i < iend; ++i)
                    if (dist[i][k] != INF)
                        for (int j = kk; j < kend; ++j)
                            relaxCell(dist[i][j], dist[i][k], dist[k][j]);
        }

        // Phase 3: all remaining blocks.
        for (int ii = 0; ii < V; ii += B) {
            if (ii == kk) continue;
            const int iend = std::min(ii + B, V);
            for (int jj = 0; jj < V; jj += B) {
                if (jj == kk) continue;
                const int jend = std::min(jj + B, V);

                for (int k = kk; k < kend; ++k)
                    for (int i = ii; i < iend; ++i)
                        if (dist[i][k] != INF)
                            for (int j = jj; j < jend; ++j)
                                relaxCell(dist[i][j], dist[i][k], dist[k][j]);
            }
        }
    }

    for (int i = 0; i < V; ++i)
        if (dist[i][i] < 0) return true;
    return false;
}

bool sameMatrix(
    const std::vector<std::vector<long long>>& a,
    const std::vector<std::vector<long long>>& b
) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].size() != b[i].size()) return false;
        for (size_t j = 0; j < a[i].size(); ++j)
            if (a[i][j] != b[i][j]) return false;
    }
    return true;
}
