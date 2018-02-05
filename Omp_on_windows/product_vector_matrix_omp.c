#include <omp.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


#define N     14000
#define CHUNKSIZE   5

/**************************************************************************/
int init_vector(double* v, int n)
{
  int i;
  double r = 1./RAND_MAX;
  srand(1);
  for (i=0;i<n;i++) {
    rand();
  }
  for (i=0;i<n;i++) {
    /* v[i] = 1; */
    v[i] = rand() * r;
  }
  return 0;
}

/**************************************************************************/
int print_vector(double* v, int n)
{
  int i;
  for (i=0;i<n;i++) {
    printf ("v[%d]=%.3g\n", i, v[i]);
  }
  return 0;
}

/**************************************************************************/
int init_matrix(double* m, int nl, int nc)
{
  int i,j;
  double r = 1./RAND_MAX;
  srand(10);
  for (i=0;i<nl;i++) {
    rand();
  }
  for (i=0;i<nl;i++) {
    for (j=0;j<nc;j++) {
      /* m[i+j*nl] = 1; */
      m[i+j*nl] = rand() * r;
    }
  }
  return 0;
}

/**************************************************************************/
int print_matrix(double* m, int nl, int nc)
{
  int i,j;
  for (i=0;i<nl;i++) {
    for (j=0;j<nc;j++) {
      printf ("%8.3g    ", m[i+j*nl]);
    }
    printf("\n");
  }
  return 0;
}

/**************************************************************************/
int main (int argc, char* argv[])
{
  int tid;
  int chunksz = CHUNKSIZE;
  int i, j;
  double* a = NULL;
  double* b = NULL;
  double* m = NULL;

  a = calloc(sizeof(double),N);
  b = calloc(sizeof(double),N);
  m = calloc(sizeof(double),N*N);
  printf("alloc: %ld Mo\n", (sizeof(double)*N+sizeof(double)*N+sizeof(double)*N*N)/(1024*1024));

  omp_set_dynamic(0);
  omp_set_num_threads(4);

  init_vector(a, N);
//  print_vector(a, N);
  init_matrix(m, N, N);
//  print_matrix(m, N, N);
#pragma omp parallel shared(chunksz, a, b, m, j) private(tid)
  {
//    double thr_time = 0.;
    long    thr_iter = 0;
    tid = omp_get_thread_num();
    printf ("Thread = %d\n", tid);

//#pragma omp barrier
//    thr_time = omp_get_wtime();

/* #pragma omp for private(i) schedule(static, chunk) */
#pragma omp for private(i, j)
    for (i=0; i<N; i++) {
      b[i] = 0.f;
      for (j=0; j<N; j++) {
        b[i] += m[i+j*N] * a[j];
        /* printf ("[%d]  b[%d] = %.3e\n", tid, i, b[i]); */
        thr_iter++;
      }
    }
//    thr_time = omp_get_wtime() - thr_time;
//    printf ("[%d] iter=%ld wtime=%f\n", tid, thr_iter, thr_time);
    printf ("[%d] iter=%ld\n", tid, thr_iter);
  }
//  print_vector(b, N);
  free(a);
  free(b);
  free(m);
  printf ("[END]\n");
  return 0;
}
