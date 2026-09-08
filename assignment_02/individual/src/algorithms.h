#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "csr.h"
#include <vector>

const long long INF=4000000000000000000LL;

struct BellmanFordResult{
std::vector<long long>distance;
bool negativeCycle;
};

BellmanFordResult bellmanFord(
const CSRGraph&graph,
int source
);

bool floydWarshall(
std::vector<std::vector<long long>>&dist
);

#endif