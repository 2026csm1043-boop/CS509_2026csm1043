#include "csr.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace {
struct InputEdge {
    int u;
    int v;
    long long w;
};
}

bool readEdgeListCSR(const std::string& filename, CSRGraph& graph) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: cannot open input file: " << filename << '\n';
        return false;
    }

    char type;
    if (!(fin >> type) || (type != 'D' && type != 'U')) {
        std::cerr << "Error: first line must be D or U.\n";
        return false;
    }

    const bool directed = (type == 'D');
    std::vector<InputEdge> edges;
    int maxVertex = -1;

    int u, v;
    long long w;
    while (fin >> u >> v >> w) {
        if (u < 0 || v < 0) {
            std::cerr << "Error: vertex IDs must be non-negative.\n";
            return false;
        }
        edges.push_back({u, v, w});
        if (u > maxVertex) maxVertex = u;
        if (v > maxVertex) maxVertex = v;
    }

    if (!fin.eof()) {
        std::cerr << "Error: malformed edge-list record. Expected: u v w.\n";
        return false;
    }

    graph.V = maxVertex + 1;
    graph.inputE = static_cast<long long>(edges.size());
    graph.directed = directed;

    // No edge means the input does not identify a maximum vertex ID.
    // The specification determines V from the maximum ID appearing in the file.
    if (edges.empty()) {
        graph.V = 0;
    }

    graph.row_ptr.assign(graph.V + 1, 0);

    // Count the CSR entries.  A non-self-loop undirected edge is stored twice;
    // an undirected self-loop is stored exactly once.
    for (const auto& e : edges) {
        ++graph.row_ptr[e.u + 1];
        if (!directed && e.u != e.v) {
            ++graph.row_ptr[e.v + 1];
        }
    }

    for (int i = 0; i < graph.V; ++i) {
        graph.row_ptr[i + 1] += graph.row_ptr[i];
    }

    graph.entries = graph.row_ptr.empty() ? 0 : graph.row_ptr.back();
    graph.col_idx.resize(static_cast<size_t>(graph.entries));
    graph.values.resize(static_cast<size_t>(graph.entries));

    // Fill in input order within each source row.  No adjacency list/matrix is
    // created; only the CSR arrays are used after the input edge list is read.
    std::vector<int> next = graph.row_ptr;
    for (const auto& e : edges) {
        int p = next[e.u]++;
        graph.col_idx[p] = e.v;
        graph.values[p] = e.w;

        if (!directed && e.u != e.v) {
            p = next[e.v]++;
            graph.col_idx[p] = e.u;
            graph.values[p] = e.w;
        }
    }

    return true;
}

void printCSR(const CSRGraph& graph) {
    std::cout << "Offset Array (row_ptr):\n";
    for (size_t i = 0; i < graph.row_ptr.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << graph.row_ptr[i];
    }
    std::cout << '\n';

    std::cout << "Intermediate Array (col_idx):\n";
    for (size_t i = 0; i < graph.col_idx.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << graph.col_idx[i];
    }
    std::cout << '\n';

    std::cout << "Property Array (values):\n";
    for (size_t i = 0; i < graph.values.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << graph.values[i];
    }
    std::cout << '\n';
}
