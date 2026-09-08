#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

#include "algorithms.h"
#include "csr.h"

using namespace std;
using namespace chrono;


bool readBellmanFordFile(
const string&filename,
vector<vector<Edge>>&adj,
int&V,
int&E,
int&source
){

ifstream file(filename);

if(!file){
cerr<<"Error: cannot open input file: "
<<filename<<endl;
return false;
}

if(!(file>>V>>E)){
cerr<<"Error: invalid Bellman-Ford input."
<<endl;
return false;
}

if(V<=0||E<0){
cerr<<"Error: invalid V or E."
<<endl;
return false;
}

adj.assign(V,{});

int countedEdges=0;

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
int weight;

if(!(file>>v>>weight)){
cerr<<"Error: invalid edge data."
<<endl;
return false;
}

if(v<0||v>=V){
cerr<<"Error: invalid neighbour vertex."
<<endl;
return false;
}

adj[u].push_back({v,weight});

countedEdges++;
}
}

string sourceWord;

if(!(file>>sourceWord>>source)){
cerr<<"Error: missing SOURCE line."
<<endl;
return false;
}

if(sourceWord!="SOURCE"){
cerr<<"Error: expected SOURCE."
<<endl;
return false;
}

if(source<0||source>=V){
cerr<<"Error: invalid source vertex."
<<endl;
return false;
}

if(countedEdges!=E){
cerr<<"Error: declared E does not match "
<<"the adjacency list."
<<endl;
return false;
}

return true;
}


bool readFloydWarshallFile(
const string&filename,
vector<vector<long long>>&dist,
int&V
){

ifstream file(filename);

if(!file){
cerr<<"Error: cannot open input file: "
<<filename<<endl;
return false;
}

if(!(file>>V)){
cerr<<"Error: invalid Floyd-Warshall input."
<<endl;
return false;
}

if(V<=0){
cerr<<"Error: invalid number of vertices."
<<endl;
return false;
}

dist.assign(
V,
vector<long long>(V)
);

for(int i=0;i<V;i++){

for(int j=0;j<V;j++){

string token;

if(!(file>>token)){
cerr<<"Error: incomplete distance matrix."
<<endl;
return false;
}

if(token=="INF"){

dist[i][j]=INF;

}
else{

try{
dist[i][j]=stoll(token);
}
catch(...){
cerr<<"Error: invalid matrix value."
<<endl;
return false;
}
}
}
}

for(int i=0;i<V;i++){

if(dist[i][i]!=0){

cerr<<"Error: diagonal entries "
<<"must be 0."
<<endl;

return false;
}
}

return true;
}


void printBellmanFordResult(
const BellmanFordResult&result,
int source
){

cout<<"Algorithm: Bellman-Ford"<<endl;

cout<<"Source: "<<source<<endl;

if(result.negativeCycle){

cout<<"Negative cycle: true"<<endl;

return;
}

cout<<"Vertex Distance"<<endl;

for(int i=0;
i<static_cast<int>(result.distance.size());
i++){

cout<<i<<" ";

if(result.distance[i]==INF){
cout<<"INF";
}
else{
cout<<result.distance[i];
}

cout<<endl;
}

cout<<"Negative cycle: none"<<endl;
}


void printFloydWarshallResult(
const vector<vector<long long>>&dist,
bool negativeCycle
){

cout<<"Algorithm: Floyd-Warshall"<<endl;

if(negativeCycle){

cout<<"Negative cycle: true"<<endl;

return;
}

cout<<"Distance matrix:"<<endl;

int V=static_cast<int>(dist.size());

for(int i=0;i<V;i++){

for(int j=0;j<V;j++){

if(dist[i][j]==INF){
cout<<"INF";
}
else{
cout<<dist[i][j];
}

if(j+1<V)
cout<<" ";
}

cout<<endl;
}

cout<<"Negative cycle: none"<<endl;
}


int main(int argc,char*argv[]){

if(argc!=3){

cerr<<"Usage:"<<endl;
cerr<<"  individual bf <input-file>"
<<endl;
cerr<<"  individual fw <input-file>"
<<endl;

return 1;
}

string algorithm=argv[1];
string filename=argv[2];


if(algorithm=="bf"){

vector<vector<Edge>>adj;

int V;
int E;
int source;

if(!readBellmanFordFile(
filename,
adj,
V,
E,
source
)){
return 1;
}


/*
CSR conversion is preprocessing.
It is NOT included in timing.
*/

CSRGraph graph=convertToCSR(adj);


/*
Algorithm timing starts here.
*/

auto start=
high_resolution_clock::now();

BellmanFordResult result=
bellmanFord(graph,source);

auto stop=
high_resolution_clock::now();


double milliseconds=
duration<double,milli>(
stop-start
).count();


printBellmanFordResult(
result,
source
);

cout<<fixed<<setprecision(3);

cout<<"Execution time: "
<<milliseconds
<<" ms"<<endl;

return 0;
}


if(algorithm=="fw"){

vector<vector<long long>>dist;

int V;

if(!readFloydWarshallFile(
filename,
dist,
V
)){
return 1;
}


/*
Matrix loading is preprocessing.
It is NOT included in timing.
*/

auto start=
high_resolution_clock::now();

bool negativeCycle=
floydWarshall(dist);

auto stop=
high_resolution_clock::now();


double milliseconds=
duration<double,milli>(
stop-start
).count();


printFloydWarshallResult(
dist,
negativeCycle
);

cout<<fixed<<setprecision(3);

cout<<"Execution time: "
<<milliseconds
<<" ms"<<endl;

return 0;
}


cerr<<"Error: unknown algorithm."
<<endl;

return 1;
}