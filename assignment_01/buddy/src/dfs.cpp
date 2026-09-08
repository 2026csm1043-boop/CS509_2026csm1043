#include "dfs.h"
#include <stack>

using namespace std;

vector<int> dfs(const CSRGraph& graph, int source) {
    vector<int> traversal;
    vector<bool> visited(graph.V, false);

    stack<int> st;

    st.push(source);

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (visited[u]) {
            continue;
        }

        visited[u] = true;
        traversal.push_back(u);

        for (int i = graph.row_ptr[u + 1] - 1;
             i >= graph.row_ptr[u];
             i--) {

            int v = graph.col_idx[i];

            if (!visited[v]) {
                st.push(v);
            }
        }
    }

    return traversal;
}