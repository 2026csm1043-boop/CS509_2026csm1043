#include "csr.h"

CSRGraph convertToCSR(const std::vector<std::vector<Edge>>& adj){

    CSRGraph graph;

    graph.vertices=static_cast<int>(adj.size());
    graph.edges=0;

    graph.row_ptr.resize(
        graph.vertices+1,
        0
    );

    for(int i=0;i<graph.vertices;i++){

        graph.row_ptr[i+1]=
            graph.row_ptr[i]+
            static_cast<int>(adj[i].size());

        graph.edges+=
            static_cast<int>(adj[i].size());
    }

    graph.col_idx.resize(graph.edges);
    graph.values.resize(graph.edges);

    int pos=0;

    for(int i=0;i<graph.vertices;i++){

        for(const Edge& e:adj[i]){

            graph.col_idx[pos]=e.vertex;
            graph.values[pos]=e.weight;

            pos++;
        }
    }

    return graph;
}