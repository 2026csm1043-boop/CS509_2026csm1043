#ifndef MAXFLOW_H
#define MAXFLOW_H

#include "csr.h"

#include <vector>

struct CutEdge
{
    int from;
    int to;
    int capacity;
};

struct MaxflowResult
{
    long long maximumFlow;
    long long minimumCutCapacity;
    std::vector<int> sourceSide;
    std::vector<int> sinkSide;
    std::vector<CutEdge> cutEdges;
};

MaxflowResult maxflowMincut(
    const CSRGraph& graph,
    int source,
    int sink
);

#endif
