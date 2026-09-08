#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "csr.h"
#include <vector>

struct MSTEdge
{
    int u;
    int v;
    int weight;
};

struct MSTResult
{
    std::vector<MSTEdge> edges;
    long long totalWeight;
    bool connected;
};

MSTResult kruskalMST(const CSRGraph& graph);

MSTResult primMST(const CSRGraph& graph);

#endif