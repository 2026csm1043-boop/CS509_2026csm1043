#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "csr.h"
#include <tuple>
#include <vector>

constexpr long long INF = 4000000000000000000LL;

struct BFSResult {
    std::vector<int> traversal;
    std::vector<int> distance;
};

struct TriangleResult {
    long long count = 0;
    std::vector<std::tuple<int,int,int>> triangles;
};

BFSResult bfs(const CSRGraph& graph, int source);

// weighted=true: use graph.values (Dijkstra, requiring non-negative weights).
// weighted=false: treat every CSR entry as unit weight.  This is used by
// the required BFS <-> SSSP validation.
std::vector<long long> sssp(const CSRGraph& graph, int source, bool weighted = true);

std::vector<int> connectedComponents(const CSRGraph& graph, int& componentCount);

TriangleResult triangleCounting(const CSRGraph& graph, bool listTriangles);

// If componentId < 0, compute BC on the whole graph.
// If componentId >= 0, only vertices belonging to that CC are considered;
// the same CSR arrays are still used directly.
std::vector<double> betweennessCentrality(
    const CSRGraph& graph,
    const std::vector<int>& component,
    int componentId = -1
);

// Build V x V APSP initialization from CSR. Parallel edges use the minimum
// weight, which is the correct initial direct-edge distance for APSP.
std::vector<std::vector<long long>> initialDistanceMatrix(const CSRGraph& graph);

// Returns true iff a negative cycle exists.
bool floydWarshall(std::vector<std::vector<long long>>& dist);

// Standard 3-phase blocked Floyd-Warshall. Handles V % B != 0.
bool blockedFloydWarshall(std::vector<std::vector<long long>>& dist, int B);

bool sameMatrix(
    const std::vector<std::vector<long long>>& a,
    const std::vector<std::vector<long long>>& b
);

#endif
