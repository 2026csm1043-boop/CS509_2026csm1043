#include "algorithms.h"

BellmanFordResult bellmanFord(
const CSRGraph& graph,
int source
){
    BellmanFordResult result;

    result.distance.assign(graph.vertices, INF);
    result.negativeCycle = false;

    result.distance[source] = 0;

    for(int pass = 0; pass < graph.vertices - 1; pass++){

        bool changed = false;

        for(int u = 0; u < graph.vertices; u++){

            if(result.distance[u] == INF)
                continue;

            for(int i = graph.row_ptr[u];
                i < graph.row_ptr[u + 1];
                i++){

                int v = graph.col_idx[i];
                long long weight = graph.values[i];

                if(result.distance[u] + weight < result.distance[v]){

                    result.distance[v] =
                        result.distance[u] + weight;

                    changed = true;
                }
            }
        }

        if(!changed)
            break;
    }

    // Extra pass for negative-cycle detection
    for(int u = 0; u < graph.vertices; u++){

        if(result.distance[u] == INF)
            continue;

        for(int i = graph.row_ptr[u];
            i < graph.row_ptr[u + 1];
            i++){

            int v = graph.col_idx[i];
            long long weight = graph.values[i];

            if(result.distance[u] + weight < result.distance[v]){

                result.negativeCycle = true;

                return result;
            }
        }
    }

    return result;
}


bool floydWarshall(
std::vector<std::vector<long long>>& dist
){
    int V = static_cast<int>(dist.size());

    for(int k = 0; k < V; k++){

        for(int i = 0; i < V; i++){

            if(dist[i][k] == INF)
                continue;

            for(int j = 0; j < V; j++){

                if(dist[k][j] == INF)
                    continue;

                long long candidate =
                    dist[i][k] + dist[k][j];

                if(candidate < dist[i][j]){
                    dist[i][j] = candidate;
                }
            }
        }
    }

    // Negative-cycle detection
    for(int i = 0; i < V; i++){

        if(dist[i][i] < 0){
            return true;
        }
    }

    return false;
}