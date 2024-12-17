#include <iostream>
#include <cuda_runtime.h>
#include <vector>
#include <algorithm>

__global__ void cudaMerge(int* input, int* output, int start, int mid, int end) {
    int i = start + blockIdx.x * blockDim.x + threadIdx.x;

    if (i < end) {
        int a = (i < mid) ? input[i] : INT_MAX;
        int b = (i >= mid && i < end) ? input[i] : INT_MAX;

        output[i] = (a < b) ? a : b;
    }
}

void cudaMergeSort(int* d_data, int* d_temp, int start, int end, int N) {
    if (end - start <= 1) return;

    int mid = (start + end) / 2;

    cudaMergeSort(d_data, d_temp, start, mid, N);
    cudaMergeSort(d_data, d_temp, mid, end, N);

    int threadsPerBlock = 256;
    int blocksPerGrid = (end - start + threadsPerBlock - 1) / threadsPerBlock;
    cudaMerge<<<blocksPerGrid, threadsPerBlock>>>(d_data, d_temp, start, mid, end);

    cudaMemcpy(d_data + start, d_temp + start, (end - start) * sizeof(int), cudaMemcpyDeviceToDevice);
}


void mergeSort(std::vector<int>& data) {
    int N = data.size();

    int* d_data;
    int* d_temp;
    cudaMalloc((void**)&d_data, N * sizeof(int));
    cudaMalloc((void**)&d_temp, N * sizeof(int));

    cudaMemcpy(d_data, data.data(), N * sizeof(int), cudaMemcpyHostToDevice);

    cudaMergeSort(d_data, d_temp, 0, N, N);

    cudaMemcpy(data.data(), d_data, N * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_data);
    cudaFree(d_temp);

}
