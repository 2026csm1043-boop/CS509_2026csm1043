#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <unordered_map>

#include "csr.h"
#include "algorithms.h"

using namespace std;
using namespace chrono;


// ------------------------------------------------------------
// Read undirected unweighted adjacency-list file
// ------------------------------------------------------------

bool readUndirectedGraph(
const string&filename,
vector<vector<Edge>>&adj,
int&V,
int&E
){

ifstream file(filename);

if(!file){

cerr<<"Error: cannot open input file: "
<<filename<<endl;

return false;
}

if(!(file>>V>>E)){

cerr<<"Error: invalid input file."<<endl;

return false;
}

if(V<=0||E<0){

cerr<<"Error: invalid V or E."<<endl;

return false;
}

adj.assign(
V,
vector<Edge>()
);

int totalEntries=0;

for(int i=0;i<V;i++){

int u;
int degree;

if(!(file>>u>>degree)){

cerr<<"Error: invalid adjacency list."
<<endl;

return false;
}

if(u!=i){

cerr<<"Error: vertices must be numbered "
<<"from 0 to V-1 in order."
<<endl;

return false;
}

if(degree<0){

cerr<<"Error: invalid degree."
<<endl;

return false;
}

for(int j=0;j<degree;j++){

int v;

if(!(file>>v)){

cerr<<"Error: invalid neighbour."
<<endl;

return false;
}

if(v<0||v>=V){

cerr<<"Error: invalid vertex "
<<v<<endl;

return false;
}

if(v==u){

cerr<<"Error: self-loops are not allowed."
<<endl;

return false;
}

adj[u].push_back({
v,
1
});

totalEntries++;
}
}

/*
    For an undirected graph every edge appears twice.

    Therefore the number of adjacency entries must be 2E.
*/

if(totalEntries!=2*E){

cerr<<"Error: adjacency list contains "
<<totalEntries
<<" entries, but 2E = "
<<2*E
<<" entries are required."
<<endl;

return false;
}


/*
    Validate that every edge appears in both directions.
*/

vector<unordered_map<int,int>>frequency(V);

for(int u=0;u<V;u++){

for(const Edge&e:adj[u]){

frequency[u][e.vertex]++;
}
}

for(int u=0;u<V;u++){

for(const Edge&e:adj[u]){

if(frequency[e.vertex][u]==0){

cerr<<"Error: graph is not undirected. "
<<"Edge "
<<u<<" -> "<<e.vertex
<<" has no reverse edge."
<<endl;

return false;
}
}
}

return true;
}


// ------------------------------------------------------------
// Create a sorted CSR graph
// ------------------------------------------------------------

CSRGraph createSortedCSR(
const vector<vector<Edge>>&adj
){

vector<vector<Edge>>sortedAdj=adj;

for(auto&list:sortedAdj){

sort(
list.begin(),
list.end(),
[](const Edge&a,const Edge&b){

return a.vertex<b.vertex;
}
);
}

return convertToCSR(sortedAdj);
}


// ------------------------------------------------------------
// Print Triangle Counting result
// ------------------------------------------------------------

void printTriangleResult(
const TriangleResult&result,
bool listTriangles
){

cout<<"Algorithm: Triangle Counting"<<endl;

cout<<"Total triangles: "
<<result.count
<<endl;

if(listTriangles){

cout<<"Triangles found:"<<endl;

for(const auto&t:result.triangles){

int a,b,c;

tie(a,b,c)=t;

cout<<"("
<<a<<", "
<<b<<", "
<<c<<")"
<<endl;
}
}
}


// ------------------------------------------------------------
// Print Betweenness Centrality result
// ------------------------------------------------------------

void printBCResult(
const vector<double>&centrality
){

cout<<"Algorithm: Betweenness Centrality"
<<endl;

cout<<"Vertex Centrality"<<endl;

cout<<fixed<<setprecision(2);

for(int i=0;
i<static_cast<int>(centrality.size());
i++){

cout<<i<<" "
<<centrality[i]
<<endl;
}
}


// ------------------------------------------------------------
// Print Connected Components result
// ------------------------------------------------------------

void printCCResult(
const vector<int>&component,
int componentCount
){

cout<<"Algorithm: Connected Components"
<<endl;

cout<<"Number of components: "
<<componentCount
<<endl;

cout<<"Vertex Component"<<endl;

for(int i=0;
i<static_cast<int>(component.size());
i++){

cout<<i<<" "
<<component[i]
<<endl;
}
}


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main(int argc,char*argv[]){

if(argc!=3){

cerr<<"Usage:"<<endl;

cerr<<"  buddy tc <input-file>"
<<endl;

cerr<<"  buddy bc <input-file>"
<<endl;

cerr<<"  buddy cc <input-file>"
<<endl;

return 1;
}

string algorithm=argv[1];

string filename=argv[2];


// ----------------------------------------------------------
// Read graph
// ----------------------------------------------------------

vector<vector<Edge>>adj;

int V;
int E;

if(!readUndirectedGraph(
filename,
adj,
V,
E
)){

return 1;
}


// ----------------------------------------------------------
// CSR conversion
//
// This is preprocessing and therefore NOT timed.
// ----------------------------------------------------------

CSRGraph graph;

if(algorithm=="tc"){

/*
    Triangle counting requires sorted
    adjacency lists.

    Sorting and CSR construction happen
    before the timer starts.
*/

graph=createSortedCSR(adj);

}
else{

graph=convertToCSR(adj);
}


// ----------------------------------------------------------
// Triangle Counting
// ----------------------------------------------------------

if(algorithm=="tc"){

/*
    Individual triangle listing is required
    only for the two smallest graph sizes.

    For this assignment those are V=10 and V=100.
*/

bool listTriangles=(V<=100);

auto start=
high_resolution_clock::now();

TriangleResult result=
triangleCounting(
graph,
listTriangles
);

auto stop=
high_resolution_clock::now();

double milliseconds=
duration<double,milli>(
stop-start
).count();

printTriangleResult(
result,
listTriangles
);

cout<<fixed<<setprecision(3);

cout<<"Execution time: "
<<milliseconds
<<" ms"
<<endl;

return 0;
}


// ----------------------------------------------------------
// Betweenness Centrality
// ----------------------------------------------------------

if(algorithm=="bc"){

auto start=
high_resolution_clock::now();

vector<double>centrality=
betweennessCentrality(
graph
);

auto stop=
high_resolution_clock::now();

double milliseconds=
duration<double,milli>(
stop-start
).count();

printBCResult(
centrality
);

cout<<"Execution time: "
<<fixed
<<setprecision(3)
<<milliseconds
<<" ms"
<<endl;

return 0;
}


// ----------------------------------------------------------
// Connected Components
// ----------------------------------------------------------

if(algorithm=="cc"){

auto start=
high_resolution_clock::now();

int componentCount=0;

vector<int>component=
connectedComponents(
graph,
componentCount
);

auto stop=
high_resolution_clock::now();

double milliseconds=
duration<double,milli>(
stop-start
).count();

printCCResult(
component,
componentCount
);

cout<<fixed<<setprecision(3);

cout<<"Execution time: "
<<milliseconds
<<" ms"
<<endl;

return 0;
}


cerr<<"Error: unknown algorithm."
<<endl;

return 1;
}