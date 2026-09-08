#include "gemm.h"

Matrix gemmSimple(const Matrix&A,const Matrix&B,int M,int K,int N){
Matrix C(M,std::vector<int>(N,0));
for(int i=0;i<M;i++){
for(int k=0;k<K;k++){
for(int j=0;j<N;j++){
C[i][j]+=A[i][k]*B[k][j];
}
}
}
return C;
}

Matrix gemmBlocking(const Matrix&A,const Matrix&B,int M,int K,int N,int blockSize){
Matrix C(M,std::vector<int>(N,0));
for(int ii=0;ii<M;ii+=blockSize){
for(int kk=0;kk<K;kk+=blockSize){
for(int jj=0;jj<N;jj+=blockSize){
int iEnd=std::min(ii+blockSize,M);
int kEnd=std::min(kk+blockSize,K);
int jEnd=std::min(jj+blockSize,N);
for(int i=ii;i<iEnd;i++){
for(int k=kk;k<kEnd;k++){
for(int j=jj;j<jEnd;j++){
C[i][j]+=A[i][k]*B[k][j];
}
}
}
}
}
}
return C;
}