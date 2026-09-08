#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include "gemm.h"
#include "csr.h"

using namespace std;
using namespace chrono;

bool readGEMM(const string&file,Matrix&A,Matrix&B,int&M,int&K,int&N){
ifstream fin(file);
if(!fin){
return false;
}
if(!(fin>>M>>K>>N)){
return false;
}
if(M<=0||K<=0||N<=0){
return false;
}
A.assign(M,vector<int>(K));
B.assign(K,vector<int>(N));
for(int i=0;i<M;i++){
for(int j=0;j<K;j++){
if(!(fin>>A[i][j])){
return false;
}
}
}
for(int i=0;i<K;i++){
for(int j=0;j<N;j++){
if(!(fin>>B[i][j])){
return false;
}
}
}
return true;
}

bool readGraph(const string&file,vector<vector<Edge>>&adj,int&V,int&E){
ifstream fin(file);
if(!fin){
return false;
}
if(!(fin>>V>>E)){
return false;
}
if(V<=0||E<0){
return false;
}
adj.assign(V,{});
for(int i=0;i<V;i++){
int u,degree;
if(!(fin>>u>>degree)){
return false;
}
if(u<0||u>=V||degree<0){
return false;
}
for(int j=0;j<degree;j++){
int v,w;
if(!(fin>>v>>w)){
return false;
}
if(v<0||v>=V){
return false;
}
adj[u].push_back({v,w});
}
}
return true;
}

void printMatrix(const Matrix&C){
for(const auto&row:C){
for(size_t j=0;j<row.size();j++){
if(j>0){
cout<<" ";
}
cout<<row[j];
}
cout<<"\n";
}
}

void printCSR(const CSRGraph&graph){
cout<<"Vertices: "<<graph.vertices<<"\n";
cout<<"Edges: "<<graph.edges<<"\n";

cout<<"row_ptr:\n";
for(size_t i=0;i<graph.row_ptr.size();i++){
if(i>0){
cout<<" ";
}
cout<<graph.row_ptr[i];
}
cout<<"\n";

cout<<"col_idx:\n";
for(size_t i=0;i<graph.col_idx.size();i++){
if(i>0){
cout<<" ";
}
cout<<graph.col_idx[i];
}
cout<<"\n";

cout<<"values:\n";
for(size_t i=0;i<graph.values.size();i++){
if(i>0){
cout<<" ";
}
cout<<graph.values[i];
}
cout<<"\n";
}

int main(int argc,char*argv[]){
if(argc<3){
cerr<<"Usage:\n";
cerr<<"./cs509 gemm-simple <input_file>\n";
cerr<<"./cs509 gemm-block <input_file> <block_size>\n";
cerr<<"./cs509 csr <input_file>\n";
return 1;
}

string algorithm=argv[1];
string file=argv[2];

if(algorithm=="gemm-simple"){
Matrix A,B;
int M,K,N;

if(!readGEMM(file,A,B,M,K,N)){
cerr<<"Error: Invalid or missing GEMM input file.\n";
return 1;
}

auto start=high_resolution_clock::now();
Matrix C=gemmSimple(A,B,M,K,N);
auto end=high_resolution_clock::now();

double timeMs=duration<double,milli>(end-start).count();

cout<<"Algorithm: GEMM Simple\n";
cout<<"Result matrix:\n";
printMatrix(C);
cout<<"Execution time: "<<timeMs<<" ms\n";
return 0;
}

if(algorithm=="gemm-block"){
if(argc<4){
cerr<<"Error: Block size is required.\n";
return 1;
}

int blockSize=atoi(argv[3]);

if(blockSize<=0){
cerr<<"Error: Block size must be positive.\n";
return 1;
}

Matrix A,B;
int M,K,N;

if(!readGEMM(file,A,B,M,K,N)){
cerr<<"Error: Invalid or missing GEMM input file.\n";
return 1;
}

auto start=high_resolution_clock::now();
Matrix C=gemmBlocking(A,B,M,K,N,blockSize);
auto end=high_resolution_clock::now();

double timeMs=duration<double,milli>(end-start).count();

cout<<"Algorithm: GEMM Blocking\n";
cout<<"Block size: "<<blockSize<<"\n";
cout<<"Result matrix:\n";
printMatrix(C);
cout<<"Execution time: "<<timeMs<<" ms\n";
return 0;
}

if(algorithm=="csr"){
vector<vector<Edge>>adj;
int V,E;

if(!readGraph(file,adj,V,E)){
cerr<<"Error: Invalid or missing graph input file.\n";
return 1;
}

CSRGraph graph=convertToCSR(adj);

cout<<"Algorithm: CSR Conversion\n";
printCSR(graph);
cout<<"Conversion completed.\n";
return 0;
}

cerr<<"Error: Unknown algorithm.\n";
return 1;
}