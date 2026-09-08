#include "maxflow.h"

#include <algorithm>
#include <climits>
#include <queue>

namespace
{
struct ResidualEdge
{
    int to;
    int rev;
    long long capacity;
};

class Dinic
{
public:
    explicit Dinic(int vertices)
        : graph(vertices), level(vertices), nextEdge(vertices)
    {
    }

    void addEdge(int from, int to, long long capacity)
    {
        ResidualEdge forward{
            to,
            static_cast<int>(graph[to].size()),
            capacity
        };

        ResidualEdge reverse{
            from,
            static_cast<int>(graph[from].size()),
            0
        };

        graph[from].push_back(forward);
        graph[to].push_back(reverse);
    }

    long long maxFlow(int source, int sink)
    {
        long long flow = 0;

        while (buildLevelGraph(source, sink))
        {
            std::fill(nextEdge.begin(), nextEdge.end(), 0);

            while (true)
            {
                const long long pushed =
                    sendFlow(source, sink, LLONG_MAX / 4);

                if (pushed == 0)
                {
                    break;
                }

                flow += pushed;
            }
        }

        return flow;
    }

    std::vector<int> reachableFrom(int source) const
    {
        std::vector<int> visited(graph.size(), 0);
        std::queue<int> q;

        visited[source] = 1;
        q.push(source);

        while (!q.empty())
        {
            const int u = q.front();
            q.pop();

            for (const auto& edge : graph[u])
            {
                if (edge.capacity > 0 && !visited[edge.to])
                {
                    visited[edge.to] = 1;
                    q.push(edge.to);
                }
            }
        }

        std::vector<int> reachable;

        for (int v = 0;
             v < static_cast<int>(graph.size());
             ++v)
        {
            if (visited[v])
            {
                reachable.push_back(v);
            }
        }

        return reachable;
    }

private:
    std::vector<std::vector<ResidualEdge>> graph;
    std::vector<int> level;
    std::vector<int> nextEdge;

    bool buildLevelGraph(int source, int sink)
    {
        std::fill(level.begin(), level.end(), -1);

        std::queue<int> q;
        level[source] = 0;
        q.push(source);

        while (!q.empty())
        {
            const int u = q.front();
            q.pop();

            for (const auto& edge : graph[u])
            {
                if (edge.capacity > 0 && level[edge.to] == -1)
                {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }

        return level[sink] != -1;
    }

    long long sendFlow(
        int u,
        int sink,
        long long pushed)
    {
        if (u == sink)
        {
            return pushed;
        }

        for (int& i = nextEdge[u];
             i < static_cast<int>(graph[u].size());
             ++i)
        {
            ResidualEdge& edge = graph[u][i];

            if (edge.capacity <= 0 ||
                level[edge.to] != level[u] + 1)
            {
                continue;
            }

            const long long allowed =
                std::min(pushed, edge.capacity);

            const long long sent =
                sendFlow(edge.to, sink, allowed);

            if (sent > 0)
            {
                edge.capacity -= sent;
                graph[edge.to][edge.rev].capacity += sent;
                return sent;
            }
        }

        return 0;
    }
};
}

MaxflowResult maxflowMincut(
    const CSRGraph& graph,
    int source,
    int sink)
{
    const int vertices =
        static_cast<int>(graph.row_ptr.size()) - 1;

    // Residual-network initialization is intentionally inside
    // this algorithm function so it belongs to the timed region.
    Dinic dinic(vertices);

    for (int u = 0; u < vertices; ++u)
    {
        for (int index = graph.row_ptr[u];
             index < graph.row_ptr[u + 1];
             ++index)
        {
            const int v = graph.col_idx[index];
            const int capacity = graph.values[index];

            if (capacity > 0)
            {
                dinic.addEdge(u, v, capacity);
            }
        }
    }

    const long long flow = dinic.maxFlow(source, sink);

    // Required minimum-cut extraction is part of the timed algorithm.
    const std::vector<int> sourceSide =
        dinic.reachableFrom(source);

    std::vector<char> isSourceSide(vertices, false);

    for (int v : sourceSide)
    {
        isSourceSide[v] = true;
    }

    std::vector<int> sinkSide;

    for (int v = 0; v < vertices; ++v)
    {
        if (!isSourceSide[v])
        {
            sinkSide.push_back(v);
        }
    }

    std::vector<CutEdge> cutEdges;
    long long cutCapacity = 0;

    for (int u = 0; u < vertices; ++u)
    {
        if (!isSourceSide[u])
        {
            continue;
        }

        for (int index = graph.row_ptr[u];
             index < graph.row_ptr[u + 1];
             ++index)
        {
            const int v = graph.col_idx[index];
            const int capacity = graph.values[index];

            if (capacity > 0 && !isSourceSide[v])
            {
                cutEdges.push_back({u, v, capacity});
                cutCapacity += capacity;
            }
        }
    }

    return {
        flow,
        cutCapacity,
        sourceSide,
        sinkSide,
        cutEdges
    };
}
