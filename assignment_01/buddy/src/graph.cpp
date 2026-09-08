#include "graph.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

bool readGraph(const string& filename, bool weighted, InputGraph& graph) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: cannot open input file." << endl;
        return false;
    }

    int V, E;

    if (!(file >> V >> E)) {
        cerr << "Error: invalid graph header." << endl;
        return false;
    }

    if (V <= 0 || E < 0) {
        cerr << "Error: invalid number of vertices or edges." << endl;
        return false;
    }

    graph.V = V;
    graph.E = E;
    graph.source = -1;
    graph.adj.assign(V, vector<Edge>());

    for (int i = 0; i < V; i++) {
        int u;
        int degree;

        if (!(file >> u >> degree)) {
            cerr << "Error: invalid adjacency list." << endl;
            return false;
        }

        if (u < 0 || u >= V || degree < 0) {
            cerr << "Error: invalid vertex or degree." << endl;
            return false;
        }

        for (int j = 0; j < degree; j++) {
            int v;
            long long w = 1;

            if (!(file >> v)) {
                cerr << "Error: invalid neighbour." << endl;
                return false;
            }

            if (v < 0 || v >= V) {
                cerr << "Error: invalid neighbour vertex." << endl;
                return false;
            }

            if (weighted) {
                if (!(file >> w)) {
                    cerr << "Error: missing edge weight." << endl;
                    return false;
                }

                if (w <= 0) {
                    cerr << "Error: SSSP requires positive edge weights." << endl;
                    return false;
                }
            }

            graph.adj[u].push_back({v, w});
        }
    }

    string sourceWord;

    if (!(file >> sourceWord)) {
        cerr << "Error: SOURCE is missing." << endl;
        return false;
    }

    if (sourceWord != "SOURCE") {
        cerr << "Error: expected SOURCE." << endl;
        return false;
    }

    if (!(file >> graph.source)) {
        cerr << "Error: invalid source vertex." << endl;
        return false;
    }

    if (graph.source < 0 || graph.source >= V) {
        cerr << "Error: source vertex out of range." << endl;
        return false;
    }

    file.close();

    return true;
}

CSRGraph convertToCSR(const InputGraph& graph) {
    CSRGraph csr;

    csr.V = graph.V;
    csr.E = graph.E;

    csr.row_ptr.resize(graph.V + 1, 0);

    for (int i = 0; i < graph.V; i++) {
        csr.row_ptr[i + 1] =
            csr.row_ptr[i] + static_cast<int>(graph.adj[i].size());
    }

    int totalEdges = csr.row_ptr[graph.V];

    csr.col_idx.resize(totalEdges);
    csr.values.resize(totalEdges);

    for (int u = 0; u < graph.V; u++) {
        int start = csr.row_ptr[u];

        for (size_t j = 0; j < graph.adj[u].size(); j++) {
            csr.col_idx[start + j] = graph.adj[u][j].to;
            csr.values[start + j] = graph.adj[u][j].weight;
        }
    }

    return csr;
}