#include "algorithms.h"

#include <algorithm>
#include <queue>
#include <numeric>

using namespace std;


// ============================================================
// DISJOINT SET UNION
// ============================================================

class DSU
{
private:
    vector<int> parent;
    vector<int> rankValue;

public:

    DSU(int n)
    {
        parent.resize(n);
        rankValue.assign(n,0);

        iota(parent.begin(),parent.end(),0);
    }

    int find(int x)
    {
        if(parent[x]!=x)
            parent[x]=find(parent[x]);

        return parent[x];
    }

    bool unite(int a,int b)
    {
        a=find(a);
        b=find(b);

        if(a==b)
            return false;

        if(rankValue[a]<rankValue[b])
            swap(a,b);

        parent[b]=a;

        if(rankValue[a]==rankValue[b])
            rankValue[a]++;

        return true;
    }
};


// ============================================================
// KRUSKAL'S MST
// ============================================================

MSTResult kruskalMST(const CSRGraph& graph)
{
    MSTResult result;

    result.totalWeight=0;
    result.connected=false;

    int V=graph.vertices;

    vector<MSTEdge> allEdges;

    /*
        CSR stores every undirected edge twice.

        Therefore we only keep the edge when u < v.
    */

    for(int u=0;u<V;u++)
    {
        int start=graph.row_ptr[u];
        int end=graph.row_ptr[u+1];

        for(int i=start;i<end;i++)
        {
            int v=graph.col_idx[i];
            int weight=graph.values[i];

            if(u<v)
            {
                allEdges.push_back(
                    {u,v,weight}
                );
            }
        }
    }


    // Sort by non-decreasing weight

    sort(
        allEdges.begin(),
        allEdges.end(),
        [](const MSTEdge& a,const MSTEdge& b)
        {
            if(a.weight!=b.weight)
                return a.weight<b.weight;

            if(a.u!=b.u)
                return a.u<b.u;

            return a.v<b.v;
        }
    );


    DSU dsu(V);


    for(const MSTEdge& edge:allEdges)
    {
        if(dsu.unite(edge.u,edge.v))
        {
            result.edges.push_back(edge);

            result.totalWeight+=edge.weight;

            if(
                static_cast<int>(
                    result.edges.size()
                )==V-1
            )
            {
                break;
            }
        }
    }


    if(V==1)
    {
        result.connected=true;
    }
    else if(
        static_cast<int>(
            result.edges.size()
        )==V-1
    )
    {
        result.connected=true;
    }

    return result;
}


// ============================================================
// PRIM'S MST
// ============================================================

MSTResult primMST(const CSRGraph& graph)
{
    MSTResult result;

    result.totalWeight=0;
    result.connected=false;

    int V=graph.vertices;

    if(V==0)
        return result;


    /*
        priority_queue entry:

        weight
        vertex
        parent
    */

    using Item=tuple<
        int,
        int,
        int
    >;

    priority_queue<
        Item,
        vector<Item>,
        greater<Item>
    > pq;


    vector<bool> visited(V,false);


    /*
        Start from vertex 0 as required
        by the assignment recommendation.
    */

    pq.push(
        make_tuple(
            0,
            0,
            -1
        )
    );


    while(!pq.empty())
    {
        auto current=pq.top();

        pq.pop();


        int weight=get<0>(current);
        int u=get<1>(current);
        int parent=get<2>(current);


        if(visited[u])
            continue;


        visited[u]=true;


        if(parent!=-1)
        {
            result.edges.push_back(
                {
                    parent,
                    u,
                    weight
                }
            );

            result.totalWeight+=weight;
        }


        int start=graph.row_ptr[u];
        int end=graph.row_ptr[u+1];


        for(int i=start;i<end;i++)
        {
            int v=graph.col_idx[i];
            int edgeWeight=graph.values[i];

            if(!visited[v])
            {
                pq.push(
                    make_tuple(
                        edgeWeight,
                        v,
                        u
                    )
                );
            }
        }
    }


    if(V==1)
    {
        result.connected=true;
    }
    else if(
        static_cast<int>(
            result.edges.size()
        )==V-1
    )
    {
        result.connected=true;
    }


    return result;
}