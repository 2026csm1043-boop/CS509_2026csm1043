#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

struct Edge {
    int to;
    long long weight;
};

struct CSRGraph {
    int V;
    int E;
    vector<int> row_ptr;
    vector<int> col_idx;
    vector<long long> values;
};

struct InputGraph {
    int V;
    int E;
    vector<vector<Edge>> adj;
    int source;
};

bool readGraph(const string& filename, bool weighted, InputGraph& graph);
CSRGraph convertToCSR(const InputGraph& graph);

#endif