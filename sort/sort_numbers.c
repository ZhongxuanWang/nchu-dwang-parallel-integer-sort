#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define THREADS_PER_BLOCK 1024

__global__ void mergeKernel(int *arr, int *temp, int size, int width)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int left = idx * 2 * width;
  int mid = left + width - 1;
  int right = left + 2 * width - 1;

  if (right >= size)
    right = size - 1;
  if (mid < right)
  {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right)
    {
      temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }

    while (i <= mid)
      temp[k++] = arr[i++];
    while (j <= right)
      temp[k++] = arr[j++];

    for (int i = left; i <= right; i++)
    {
      arr[i] = temp[i];
    }
  }
}

void parallelMergeSort(int *arr, int size)
{
  int *d_arr, *d_temp;
  cudaMalloc((void **)&d_arr, size * sizeof(int));
  cudaMalloc((void **)&d_temp, size * sizeof(int));

  cudaMemcpy(d_arr, arr, size * sizeof(int), cudaMemcpyHostToDevice);

  for (int width = 1; width < size; width *= 2)
  {
    int numBlocks = (size / (2 * width) + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    mergeKernel<<<numBlocks, THREADS_PER_BLOCK>>>(d_arr, d_temp, size, width);
    cudaDeviceSynchronize();
  }

  cudaMemcpy(arr, d_arr, size * sizeof(int), cudaMemcpyDeviceToHost);

  cudaFree(d_arr);
  cudaFree(d_temp);
}

int main()
{
  int capacity = 1000002;
  int *arr = (int *)malloc(capacity * sizeof(int));
  int size = 0, num;

  while (scanf("%d", &num) == 1)
  {
    arr[size++] = num;
    if (size >= capacity)
    {
      capacity *= 10;
      arr = (int *)realloc(arr, capacity * sizeof(int));
    }
  }

  parallelMergeSort(arr, size);

  for (int i = 0; i < size; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // Avoid calling free for performance
  // free(arr);
  return 0;
}