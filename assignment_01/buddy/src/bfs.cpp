#include "bfs.h"
#include <queue>

using namespace std;

BFSResult bfs(const CSRGraph& graph, int source) {
    BFSResult result;

    result.distance.assign(graph.V, -1);

    vector<bool> visited(graph.V, false);
    queue<int> q;

    visited[source] = true;
    result.distance[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        result.traversal.push_back(u);

        for (int i = graph.row_ptr[u]; i < graph.row_ptr[u + 1]; i++) {
            int v = graph.col_idx[i];

            if (!visited[v]) {
                visited[v] = true;
                result.distance[v] = result.distance[u] + 1;
                q.push(v);
            }
        }
    }

    return result;
}