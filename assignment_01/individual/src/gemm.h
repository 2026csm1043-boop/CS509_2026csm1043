#ifndef GEMM_H
#define GEMM_H

#include <vector>

using Matrix=std::vector<std::vector<int>>;

Matrix gemmSimple(const Matrix&A,const Matrix&B,int M,int K,int N);
Matrix gemmBlocking(const Matrix&A,const Matrix&B,int M,int K,int N,int blockSize);

#endif