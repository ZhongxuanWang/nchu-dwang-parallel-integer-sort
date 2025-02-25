#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void mergeSort(int arr[], int temp[], int size);
void merge(int arr[], int temp[], int left, int mid, int right);

int main()
{
  int capacity = 1000002;
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

  int *temp = malloc(capacity * sizeof(int));
  mergeSort(arr, temp, size);

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

void mergeSort(int arr[], int temp[], int size)
{
  for (int width = 1; width < size; width *= 2)
  {
    for (int i = 0; i < size; i += 2 * width)
    {
      int left = i;
      int mid = i + width - 1;
      int right = (i + 2 * width - 1 < size) ? (i + 2 * width - 1) : (size - 1);
      if (mid < right)
      {
        merge(arr, temp, left, mid, right);
      }
    }
  }
}

void merge(int arr[], int temp[], int left, int mid, int right)
{
  int i = left, j = mid + 1, k = left;

  while (i <= mid && j <= right)
  {
    temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
  }

  // chooses whichever one is faster based on the input size lol
  if (mid - i + 1 <= 8)
  {
    while (i <= mid)
      temp[k++] = arr[i++];
  }
  else
  {
    memcpy(&temp[k], &arr[i], (mid - i + 1) * sizeof(int));
    k += (mid - i + 1);
    i = mid + 1;
  }

  if (right - j + 1 <= 8)
  {
    while (j <= right)
      temp[k++] = arr[j++];
  }
  else
  {
    memcpy(&temp[k], &arr[j], (right - j + 1) * sizeof(int));
  }

  // faster than direct copying for large arrays!
  if (right - left <= 8)
  {
    for (int i = left; i <= right; i++)
    {
      arr[i] = temp[i];
    }
  }
  else
  {
    memcpy(&arr[left], &temp[left], (right - left + 1) * sizeof(int));
  }
}
