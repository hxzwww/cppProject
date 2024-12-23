#include <iostream>
#include <cuda_runtime.h>
#include <vector>
#include <algorithm>

__global__ void merge_kernel(int *input, int *output, int size, int width) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int left = tid * (2 * width);
    int mid = min(left + width, size);
    int right = min(left + 2 * width, size);

    int i = left, j = mid, k = left;

    while (i < mid && j < right) {
        if (input[i] < input[j]) {
            output[k++] = input[i++];
        } else {
            output[k++] = input[j++];
        }
    }

    while (i < mid) {
        output[k++] = input[i++];
    }

    while (j < right) {
        output[k++] = input[j++];
    }
}

void merge_sort_cuda(int *data, int size) {
    int *d_input, *d_output;
    cudaMalloc(&d_input, size * sizeof(int));
    cudaMalloc(&d_output, size * sizeof(int));

    cudaMemcpy(d_input, data, size * sizeof(int), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (size + blockSize - 1) / blockSize;

    for (int width = 1; width < size; width *= 2) {
        merge_kernel<<<numBlocks, blockSize>>>(d_input, d_output, size, width);
        cudaDeviceSynchronize();

        int *temp = d_input;
        d_input = d_output;
        d_output = temp;
    }

    cudaMemcpy(data, d_input, size * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_input);
    cudaFree(d_output);
}

