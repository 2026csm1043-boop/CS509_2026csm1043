#ifndef CSR_H
#define CSR_H

#include <vector>

struct Edge{
int vertex;
int weight;
};

struct CSRGraph{
int vertices;
int edges;
std::vector<int>row_ptr;
std::vector<int>col_idx;
std::vector<int>values;
};

CSRGraph convertToCSR(const std::vector<std::vector<Edge>>&adj);

#endif