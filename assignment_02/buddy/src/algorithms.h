#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "csr.h"
#include <vector>
#include <tuple>

struct TriangleResult{
long long count;
std::vector<std::tuple<int,int,int>>triangles;
};

TriangleResult triangleCounting(
const CSRGraph&graph,
bool listTriangles
);

std::vector<double>betweennessCentrality(
const CSRGraph&graph
);

std::vector<int>connectedComponents(
const CSRGraph&graph,
int&componentCount
);

#endif