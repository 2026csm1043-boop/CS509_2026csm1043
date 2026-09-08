#ifndef CSR_H
#define CSR_H

#include <string>
#include <utility>
#include <vector>

struct CSRGraph
{
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
    std::vector<int> values;
};

using WeightedAdjList =
    std::vector<std::vector<std::pair<int, int>>>;

// Convert a weighted adjacency list to CSR.
// For Assignment 3 Maxflow-Mincut, values stores capacities.
CSRGraph convertToCSR(const WeightedAdjList& adjacencyList);

#endif
