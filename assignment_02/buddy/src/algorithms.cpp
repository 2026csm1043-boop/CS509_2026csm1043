#include "algorithms.h"

#include <algorithm>
#include <queue>
#include <vector>
#include <tuple>


// ------------------------------------------------------------
// Triangle Counting
// ------------------------------------------------------------

TriangleResult triangleCounting(
const CSRGraph&graph,
bool listTriangles
){

TriangleResult result;

result.count=0;

int V=graph.vertices;

/*
    The adjacency lists are sorted first.

    This is done before the timed algorithm call
    in the driver by making a sorted CSR copy.

    Each triangle is found at all three vertices,
    so the raw count is divided by 3.
*/

for(int u=0;u<V;u++){

    int start=graph.row_ptr[u];
    int end=graph.row_ptr[u+1];

    for(int i=start;i<end;i++){

        int v=graph.col_idx[i];

        if(v<=u)
            continue;

        int p1=start;
        int p2=graph.row_ptr[v];

        int end1=end;
        int end2=graph.row_ptr[v+1];

        while(p1<end1 && p2<end2){

            int a=graph.col_idx[p1];
            int b=graph.col_idx[p2];

            if(a==b){

                if(a>v){

                    result.count++;

                    if(listTriangles){

                        int x=u;
                        int y=v;
                        int z=a;

                        if(x>y)
                            std::swap(x,y);

                        if(y>z)
                            std::swap(y,z);

                        if(x>y)
                            std::swap(x,y);

                        result.triangles.push_back(
                            std::make_tuple(x,y,z)
                        );
                    }
                }

                p1++;
                p2++;
            }
            else if(a<b){

                p1++;
            }
            else{

                p2++;
            }
        }
    }
}

/*
    With the ordering restriction u < v < w,
    every triangle has already been counted exactly once.

    Therefore no division by 3 is needed here.
*/

return result;
}


// ------------------------------------------------------------
// Betweenness Centrality
// Brandes algorithm for unweighted undirected graphs
// ------------------------------------------------------------

std::vector<double>betweennessCentrality(
const CSRGraph&graph
){

int V=graph.vertices;

std::vector<double>CB(V,0.0);

/*
    Brandes algorithm.

    For every source s:
      1. BFS
      2. Count shortest paths
      3. Back-propagate dependencies
*/

for(int s=0;s<V;s++){

    std::vector<std::vector<int>>P(V);

    std::vector<int>distance(
        V,
        -1
    );

    std::vector<double>sigma(
        V,
        0.0
    );

    std::vector<int>S;

    std::queue<int>Q;

    sigma[s]=1.0;
    distance[s]=0;

    Q.push(s);

    while(!Q.empty()){

        int v=Q.front();
        Q.pop();

        S.push_back(v);

        for(int i=graph.row_ptr[v];
            i<graph.row_ptr[v+1];
            i++){

            int w=graph.col_idx[i];

            if(distance[w]<0){

                distance[w]=distance[v]+1;

                Q.push(w);
            }

            if(distance[w]==distance[v]+1){

                sigma[w]+=sigma[v];

                P[w].push_back(v);
            }
        }
    }

    std::vector<double>delta(
        V,
        0.0
    );

    while(!S.empty()){

        int w=S.back();
        S.pop_back();

        for(int v:P[w]){

            if(sigma[w]!=0){

                delta[v]+=
                    (sigma[v]/sigma[w])
                    *
                    (1.0+delta[w]);
            }
        }

        if(w!=s){

            CB[w]+=delta[w];
        }
    }
}

/*
    For undirected graphs, every pair is encountered
    in both directions by Brandes' algorithm.

    Therefore divide by 2 to obtain the raw
    unnormalized undirected betweenness centrality.
*/

for(int i=0;i<V;i++){

    CB[i]/=2.0;
}

return CB;
}


// ------------------------------------------------------------
// Connected Components
// BFS on CSR graph
// ------------------------------------------------------------

std::vector<int>connectedComponents(
const CSRGraph&graph,
int&componentCount
){

int V=graph.vertices;

std::vector<int>component(
    V,
    -1
);

componentCount=0;

std::queue<int>Q;

for(int start=0;start<V;start++){

    if(component[start]!=-1)
        continue;

    component[start]=componentCount;

    Q.push(start);

    while(!Q.empty()){

        int u=Q.front();
        Q.pop();

        for(int i=graph.row_ptr[u];
            i<graph.row_ptr[u+1];
            i++){

            int v=graph.col_idx[i];

            if(component[v]==-1){

                component[v]=componentCount;

                Q.push(v);
            }
        }
    }

    componentCount++;
}

return component;
}