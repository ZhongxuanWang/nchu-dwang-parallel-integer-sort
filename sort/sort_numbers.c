#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 8

typedef struct {
  int *arr;
  int left;
  int right;
  int depth;
} thread_args;

void merge(int arr[], int left, int mid, int right) {
    int size = right - left + 1;
    int *temp = (int *)malloc(size * sizeof(int));

    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right)
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (i = 0; i < size; i++)
        arr[left + i] = temp[i];
}

void *mergeSort(void *arg) {
    thread_args *data = (thread_args *)arg;
    int left = data->left;
    int right = data->right;
    int depth = data->depth;
    int *arr = data->arr;

    if (left < right) {
        int mid = (left + right) / 2;

        if (depth < MAX_THREADS) {
            pthread_t thread1, thread2;
            thread_args data1 = {arr, left, mid, depth + 1};
            thread_args data2 = {arr, mid + 1, right, depth + 1};

            pthread_create(&thread1, NULL, mergeSort, &data1);
            pthread_create(&thread2, NULL, mergeSort, &data2);

            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
        } else {
            thread_args data1 = {arr, left, mid, depth + 1};
            thread_args data2 = {arr, mid + 1, right, depth + 1};

            mergeSort(&data1);
            mergeSort(&data2);
        }

        merge(arr, left, mid, right);
    }

    return NULL;
}

int main()
{
  int capacity = 1_000_002;
  int *arr = malloc(capacity * sizeof(int));
  int size = 0, num;

  while (scanf("%d", &num) == 1)
  {
    arr[size++] = num;
    if (size >= capacity)
    {
      capacity *= 10;
      arr = realloc(arr, capacity * sizeof(int));
    }
  }
  int n = sizeof(arr) / sizeof(arr[0]);
  thread_args data = {arr, 0, n - 1, 0};
  mergeSort(&data);

  for (int i = 0; i < size; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // avoid calling it to make it faster..
  // free(arr);
  // free(temp);
  return 0;
}
