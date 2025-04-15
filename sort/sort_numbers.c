#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// #include <omp.h>

typedef struct {
  int *arr;
  int *temp_arr; // Need temp_arr pointer here
  int left;
  int right;
  int depth;
} thread_args;

void mergeSort_serial(int arr[], int temp_arr[], int left, int right);
void merge(int arr[], int temp_arr[], int left, int mid, int right);


void merge(int arr[], int temp_arr[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp_arr[k++] = arr[i++];
        } else {
            temp_arr[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp_arr[k++] = arr[i++];
    }
    while (j <= right) {
        temp_arr[k++] = arr[j++];
    }
    for (k = left; k <= right; k++) {
        arr[k] = temp_arr[k];
    }
    // No malloc/free here
}

void mergeSort_serial(int arr[], int temp_arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort_serial(arr, temp_arr, left, mid);
        mergeSort_serial(arr, temp_arr, mid + 1, right);
        merge(arr, temp_arr, left, mid, right); // Pass temp_arr
    }
}

void *mergeSort(void *arg) {
  thread_args *data = (thread_args *)arg;
  int left = data->left;
  int right = data->right;
  int depth = data->depth;
  int *arr = data->arr;
  int *temp_arr = data->temp_arr; // Get temp_arr from args

  if (left >= right) {
      return NULL;
  }

  if ((right - left + 1) < 4096) {
      mergeSort_serial(arr, temp_arr, left, right); // Pass temp_arr
      return NULL;
  }

  int mid = left + (right - left) / 2;

  if (depth < 4) {
      pthread_t thread1, thread2;
      // Pass temp_arr in thread_args
      thread_args data1 = {arr, temp_arr, left, mid, depth + 1};
      thread_args data2 = {arr, temp_arr, mid + 1, right, depth + 1};
      int ret1, ret2;

      ret1 = pthread_create(&thread1, NULL, mergeSort, &data1);
      ret2 = pthread_create(&thread2, NULL, mergeSort, &data2);
      pthread_join(thread1, NULL);
      pthread_join(thread2, NULL);

  } else {
      mergeSort_serial(arr, temp_arr, left, mid);
      mergeSort_serial(arr, temp_arr, mid + 1, right);
  }

  merge(arr, temp_arr, left, mid, right);

  return NULL;
}

int main()
{
  int capacity = 1000002;
  int *arr = malloc(capacity * sizeof(int));
   if (arr == NULL) { // Basic error check
      perror("Failed to allocate initial array");
      return 1;
   }
  int size = 0, num;

  while (scanf("%d", &num) == 1)
  {

    if (size >= capacity)
    {
      capacity *= 10;
      int *new_arr = realloc(arr, capacity * sizeof(int));
       if (new_arr == NULL) {
           perror("Failed to reallocate array");
           free(arr); // Free old buffer if realloc fails
           return 1;
       }
       arr = new_arr;
    }
    arr[size++] = num; // Assign after potential realloc
  }
  int *temp_arr = malloc(size * sizeof(int));

  thread_args data = {arr, temp_arr, 0, size - 1, 0};
  mergeSort(&data);

  for (int i = 0; i < size; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // free(arr);
  // free(temp_arr); // If freeing arr, should also free temp_arr
  return 0;
}