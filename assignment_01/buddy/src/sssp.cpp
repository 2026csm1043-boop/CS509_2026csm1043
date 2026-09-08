#include "sssp.h"
#include <queue>
#include <utility>

using namespace std;

vector<long long> sssp(const CSRGraph& graph, int source) {
    vector<long long> distance(graph.V, INF);

    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        long long currentDistance = pq.top().first;
        int u = pq.top().second;

        pq.pop();

        if (currentDistance != distance[u]) {
            continue;
        }

        for (int i = graph.row_ptr[u]; i < graph.row_ptr[u + 1]; i++) {
            int v = graph.col_idx[i];
            long long weight = graph.values[i];

            long long newDistance = currentDistance + weight;

            if (newDistance < distance[v]) {
                distance[v] = newDistance;
                pq.push({newDistance, v});
            }
        }
    }

    return distance;
}