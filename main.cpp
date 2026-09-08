#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

#include "csr.h"
#include "algorithms.h"

using namespace std;
using namespace chrono;


// ============================================================
// READ AND VALIDATE MST GRAPH
// ============================================================

bool readMSTGraph(
    const string& filename,
    vector<vector<Edge>>& adj,
    int& V,
    int& E
)
{
    ifstream file(filename);

    if(!file)
    {
        cerr<<"Error: cannot open input file: "
            <<filename<<endl;

        return false;
    }


    if(!(file>>V>>E))
    {
        cerr<<"Error: invalid first line."
            <<endl;

        return false;
    }


    if(V<=0)
    {
        cerr<<"Error: number of vertices must be positive."
            <<endl;

        return false;
    }


    if(E<0)
    {
        cerr<<"Error: number of edges cannot be negative."
            <<endl;

        return false;
    }


    adj.assign(
        V,
        vector<Edge>()
    );


    int totalEntries=0;


    for(int i=0;i<V;i++)
    {
        int u;
        int degree;


        if(!(file>>u>>degree))
        {
            cerr<<"Error: invalid adjacency list."
                <<endl;

            return false;
        }


        if(u!=i)
        {
            cerr<<"Error: expected vertex "
                <<i
                <<" but found "
                <<u
                <<endl;

            return false;
        }


        if(degree<0)
        {
            cerr<<"Error: negative degree."
                <<endl;

            return false;
        }


        for(int j=0;j<degree;j++)
        {
            int v;
            int weight;


            if(!(file>>v>>weight))
            {
                cerr<<"Error: invalid edge."
                    <<endl;

                return false;
            }


            if(v<0 || v>=V)
            {
                cerr<<"Error: invalid vertex "
                    <<v
                    <<endl;

                return false;
            }


            if(v==u)
            {
                cerr<<"Error: self-loop detected at "
                    <<u
                    <<endl;

                return false;
            }


            adj[u].push_back(
                {
                    v,
                    weight
                }
            );


            totalEntries++;
        }
    }


    /*
        Every undirected edge must occur
        twice in the adjacency lists.
    */

    if(totalEntries!=2*E)
    {
        cerr<<"Error: expected "
            <<2*E
            <<" adjacency entries but found "
            <<totalEntries
            <<endl;

        return false;
    }


    /*
        Check that every edge has a matching
        reverse edge with the same weight.
    */

    for(int u=0;u<V;u++)
    {
        for(const Edge& e:adj[u])
        {
            bool reverseFound=false;


            for(const Edge& reverse:adj[e.vertex])
            {
                if(
                    reverse.vertex==u &&
                    reverse.weight==e.weight
                )
                {
                    reverseFound=true;
                    break;
                }
            }


            if(!reverseFound)
            {
                cerr<<"Error: edge "
                    <<u
                    <<" -> "
                    <<e.vertex
                    <<" does not have a matching "
                    <<"reverse edge with the same weight."
                    <<endl;

                return false;
            }
        }
    }


    /*
        MST graph must be connected.

        We let the algorithms determine this,
        because connectivity checking itself
        must not be part of the measured
        algorithm time.
    */


    return true;
}


// ============================================================
// PRINT MST RESULT
// ============================================================

void printMST(
    const string& algorithm,
    const MSTResult& result
)
{
    cout<<"Algorithm: "
        <<algorithm
        <<endl;


    if(!result.connected)
    {
        cout<<"Error: graph is not connected."
            <<endl;

        return;
    }


    cout<<"MST edges:"
        <<endl;


    for(const MSTEdge& e:result.edges)
    {
        cout<<e.u
            <<" "
            <<e.v
            <<" "
            <<e.weight
            <<endl;
    }


    cout<<"Total MST weight: "
        <<result.totalWeight
        <<endl;
}


// ============================================================
// MAIN
// ============================================================

int main(
    int argc,
    char* argv[]
)
{
    /*
        Usage:

        individual.exe mst-k mst_10.txt

        individual.exe mst-p mst_10.txt

        individual.exe mst-both mst_10.txt
    */


    if(argc!=3)
    {
        cerr<<"Usage:"<<endl;

        cerr<<"  individual.exe mst-k <input-file>"
            <<endl;

        cerr<<"  individual.exe mst-p <input-file>"
            <<endl;

        cerr<<"  individual.exe mst-both <input-file>"
            <<endl;

        return 1;
    }


    string algorithm=argv[1];

    string filename=argv[2];


    if(
        algorithm!="mst-k" &&
        algorithm!="mst-p" &&
        algorithm!="mst-both"
    )
    {
        cerr<<"Error: unknown algorithm."
            <<endl;

        return 1;
    }


    // ========================================================
    // READ INPUT
    // ========================================================

    vector<vector<Edge>> adj;

    int V;
    int E;


    if(!readMSTGraph(
        filename,
        adj,
        V,
        E
    ))
    {
        return 1;
    }


    // ========================================================
    // CSR CONVERSION
    //
    // This is NOT TIMED.
    //
    // This calls your Assignment-2 CSR function.
    // ========================================================

    CSRGraph graph=
        convertToCSR(adj);


    // ========================================================
    // KRUSKAL
    // ========================================================

    if(
        algorithm=="mst-k" ||
        algorithm=="mst-both"
    )
    {
        auto start=
            high_resolution_clock::now();


        MSTResult result=
            kruskalMST(graph);


        auto stop=
            high_resolution_clock::now();


        double milliseconds=
            duration<double,milli>(
                stop-start
            ).count();


        printMST(
            "Kruskal's MST",
            result
        );


        cout<<fixed<<setprecision(3);

        cout<<"Execution time: "
            <<milliseconds
            <<" ms"
            <<endl;


        if(algorithm=="mst-both")
            cout<<endl;
    }


    // ========================================================
    // PRIM
    // ========================================================

    if(
        algorithm=="mst-p" ||
        algorithm=="mst-both"
    )
    {
        auto start=
            high_resolution_clock::now();


        MSTResult result=
            primMST(graph);


        auto stop=
            high_resolution_clock::now();


        double milliseconds=
            duration<double,milli>(
                stop-start
            ).count();


        printMST(
            "Prim's MST",
            result
        );


        cout<<fixed<<setprecision(3);

        cout<<"Execution time: "
            <<milliseconds
            <<" ms"
            <<endl;
    }


    return 0;
}