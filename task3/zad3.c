#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define MIN_TASK_SIZE 100
void fill_arrays(int* a1, int* a2, int size) {
  srand(time(NULL));
  for (int i = 0; i < size; i++) {
    a1[i] = rand();
    a2[i] = a1[i];
  }
}

void merge(int* X, int n, int* tmp) {
  int i = 0;
  int j = n / 2;
  int ti = 0;

  while (i < n / 2 && j < n) {
    if (X[i] < X[j]) {
      tmp[ti] = X[i];
      ti++;
      i++;
    } else {
      tmp[ti] = X[j];
      ti++;
      j++;
    }
  }
  while (i < n / 2) {
    tmp[ti] = X[i];
    ti++;
    i++;
  }
  while (j < n) {
    tmp[ti] = X[j];
    ti++;
    j++;
  }
  memcpy(X, tmp, n * sizeof(int));
}

void mergeSort(int* X, int n, int* tmp) {
  if (n < 2)
    return;

#pragma omp task shared(X) if (n > MIN_TASK_SIZE)
  mergeSort(X, n / 2, tmp);

#pragma omp task shared(X) if (n > MIN_TASK_SIZE)
  mergeSort(X + (n / 2), n - (n / 2), tmp + n / 2);

#pragma omp taskwait
  merge(X, n, tmp);
}

int default_cmp(const void* a, const void* b) {
  return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {
  int N = atoi(argv[1]);
  int numThreads = atoi(argv[2]);
  int* X1 = (int*)malloc(N * sizeof(int));
  int* X2 = (int*)malloc(N * sizeof(int));
  int* tmp = (int*)malloc(N * sizeof(int));

  omp_set_dynamic(0);
  omp_set_num_threads(numThreads);

  fill_arrays(X1, X2, N);

  double begin1 = omp_get_wtime();
#pragma omp parallel
  {
#pragma omp single
    mergeSort(X1, N, tmp);
  }
  double end1 = omp_get_wtime();
  printf("merge sort: %lf\n", end1 - begin1);

  double begin2 = omp_get_wtime();
  qsort(X2, N, sizeof(int), default_cmp);
  double end2 = omp_get_wtime();
  printf("qsort: %lf\n", end2 - begin2);

  for (int i = 0; i < N; i++)
    if (X1[i] != X2[i]) {
      printf("merge sort wrong, ending");
      return 1;
    }
  free(X1);
  free(X2);
  free(tmp);
  return 0;
}
