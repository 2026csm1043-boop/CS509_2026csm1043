#ifndef CSR_H
#define CSR_H

#include <string>
#include <vector>

struct CSRGraph {
    int V = 0;
    long long inputE = 0;     // number of edges in the input file
    long long entries = 0;    // number of CSR entries
    bool directed = true;

    std::vector<int> row_ptr;
    std::vector<int> col_idx;
    std::vector<long long> values;
};

// Task 1: reads the D/U edge-list format and constructs CSR directly.
bool readEdgeListCSR(const std::string& filename, CSRGraph& graph);

void printCSR(const CSRGraph& graph);

#endif
