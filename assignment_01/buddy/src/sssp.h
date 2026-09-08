#ifndef SSSP_H
#define SSSP_H

#include "graph.h"
#include <vector>

using namespace std;

const long long INF = 4000000000000000000LL;

vector<long long> sssp(const CSRGraph& graph, int source);

#endif