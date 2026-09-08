#include "csr.h"

CSRGraph convertToCSR(const WeightedAdjList& adjacencyList)
{
    CSRGraph graph;

    const int vertices = static_cast<int>(adjacencyList.size());

    graph.row_ptr.resize(vertices + 1, 0);

    for (int u = 0; u < vertices; ++u)
    {
        graph.row_ptr[u + 1] =
            graph.row_ptr[u] +
            static_cast<int>(adjacencyList[u].size());
    }

    const int storedEdges = graph.row_ptr[vertices];

    graph.col_idx.reserve(storedEdges);
    graph.values.reserve(storedEdges);

    for (int u = 0; u < vertices; ++u)
    {
        for (const auto& edge : adjacencyList[u])
        {
            graph.col_idx.push_back(edge.first);
            graph.values.push_back(edge.second);
        }
    }

    return graph;
}
