// RUN: %clang_cppamp -Werror -c %s

#include <stdlib.h>
#include <iostream>
#include <functional>
#include <vector>
#include <amp.h>
using namespace Concurrency;

#define N 10

void randomInit(std::vector<float>& data, int n)
{
    for (int i = 0; i < n; ++i)
	data[i] = rand() / (float)RAND_MAX;
}

void vectorAdd_by_array(const std::vector<float>& vecA, const std::vector<float>& vecB){
   array<float> A(N, vecA.begin());
   array<float> B(N, vecB.begin());
   array<float> C(N);
   extent<1> e(N);

   parallel_for_each(e,
         [&](index<1> idx) restrict(amp) { C[idx] = A[idx] + B[idx]; }); 
}

int main(void)
{
    std::vector<float> vecA(N);
    std::vector<float> vecB(N);
    randomInit(vecA, N);
    randomInit(vecB, N);

    vectorAdd_by_array(vecA, vecB);

    return 0;
}



