#include "pagerank.h"

#include <cmath>

PageRankResult pageRank(const CSRGraph& graph,
                        double damping,
                        double tolerance,
                        int maxIterations) {
    const int n = graph.vertices;

    PageRankResult result;
    result.ranks.clear();
    result.iterations = 0;
    result.converged = false;

    if (n <= 0) {
        result.converged = true;
        return result;
    }

    std::vector<double> current(n, 1.0 / static_cast<double>(n));
    std::vector<double> next(n, 0.0);

    // Precompute outdegrees from CSR.
    std::vector<int> outDegree(n);
    for (int v = 0; v < n; ++v) {
        outDegree[v] = graph.row_ptr[v + 1] - graph.row_ptr[v];
    }

    for (int iteration = 1; iteration <= maxIterations; ++iteration) {
        const double base = (1.0 - damping) / static_cast<double>(n);

        // Start every vertex with the teleportation contribution.
        std::fill(next.begin(), next.end(), base);

        // Distribute rank from every vertex.
        for (int u = 0; u < n; ++u) {
            if (outDegree[u] == 0) {
                // Dangling vertex: distribute its complete rank
                // uniformly to every vertex.
                const double contribution =
                    damping * current[u] / static_cast<double>(n);

                for (int v = 0; v < n; ++v) {
                    next[v] += contribution;
                }
            } else {
                const double contribution =
                    damping * current[u] /
                    static_cast<double>(outDegree[u]);

                for (int p = graph.row_ptr[u];
                     p < graph.row_ptr[u + 1]; ++p) {
                    const int v = graph.col_idx[p];
                    next[v] += contribution;
                }
            }
        }

        double change = 0.0;
        for (int v = 0; v < n; ++v) {
            change += std::abs(next[v] - current[v]);
        }

        current.swap(next);
        result.iterations = iteration;

        if (change <= tolerance) {
            result.converged = true;
            break;
        }
    }

    result.ranks = std::move(current);
    return result;
}
