// cuda_sum.cu
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <time.h>

#define NUM_ELEMENTS 100000000
#define THREADS_PER_BLOCK 256

__global__ void sum_reduction(int *input, long long *output, int n) {
    __shared__ long long shared_data[THREADS_PER_BLOCK];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

    // Carrega os dados na memória compartilhada
    long long temp = 0;
    if (i < n)
        temp = input[i];
    shared_data[tid] = temp;
    __syncthreads();

    // Redução na memória compartilhada
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s && (i + s) < n) {
            shared_data[tid] += shared_data[tid + s];
        }
        __syncthreads();
    }

    // Escreve o resultado deste bloco na memória global
    if (tid == 0) {
        output[blockIdx.x] = shared_data[0];
    }
}

int main() {
    int *h_data = (int *) malloc(NUM_ELEMENTS * sizeof(int));
    int *d_data;
    long long *d_partial_sums, *h_partial_sums;
    long long total_sum = 0;

    // Gera números aleatórios
    printf("Gerando números aleatórios...\n");
    srand(time(NULL));
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        h_data[i] = rand() % 100 + 1;
    }

    // Aloca memória no dispositivo
    cudaMalloc((void**)&d_data, NUM_ELEMENTS * sizeof(int));

    int blocks = (NUM_ELEMENTS + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    cudaMalloc((void**)&d_partial_sums, blocks * sizeof(long long));
    h_partial_sums = (long long*) malloc(blocks * sizeof(long long));

    // Copia os dados para o dispositivo
    cudaMemcpy(d_data, h_data, NUM_ELEMENTS * sizeof(int), cudaMemcpyHostToDevice);

    // Lança o kernel e mede o tempo
    printf("\nCalculando a soma usando CUDA...\n");
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    sum_reduction<<<blocks, THREADS_PER_BLOCK>>>(d_data, d_partial_sums, NUM_ELEMENTS);

    // Copia as somas parciais de volta para o host
    cudaMemcpy(h_partial_sums, d_partial_sums, blocks * sizeof(long long), cudaMemcpyDeviceToHost);

    // Redução final no host
    for (int i = 0; i < blocks; i++) {
        total_sum += h_partial_sums[i];
    }

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    printf("Soma Total: %lld\n", total_sum);
    printf("Tempo levado usando CUDA: %.2f segundos\n", milliseconds / 1000.0);

    // Libera memória
    cudaFree(d_data);
    cudaFree(d_partial_sums);
    free(h_data);
    free(h_partial_sums);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}
