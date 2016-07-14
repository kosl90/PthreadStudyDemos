#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  double* a;
  double* b;
  double sum;
  int veclen;
} DOTDATA;

#define NUMTHREADS 4
#define VECLEN 100

DOTDATA dotstr;
pthread_t callThd[NUMTHREADS];
pthread_mutex_t mutexsum;

void* dotprod(void* arg)
{
  int i, start, end, len;
  long offset;
  double mysum, *x, *y;
  offset = (long)arg;

  len = dotstr.veclen;
  start = offset*len;
  end = start + len;
  x = dotstr.a;
  y = dotstr.b;

  mysum = 0;
  for (i = start; i < end; ++i) {
    mysum += (x[i] * y[i]);
  }

  pthread_mutex_lock(&mutexsum);
  dotstr.sum += mysum;
  pthread_mutex_unlock(&mutexsum);
  pthread_exit((void*)0);
}

int main(int argc, char const *argv[]) {
  long i;
  double *a, *b;
  void* status;
  pthread_attr_t attr;

  a = (double*)malloc(NUMTHREADS*VECLEN*sizeof(double));
  b = (double*)malloc(NUMTHREADS*VECLEN*sizeof(double));

  for (i = 0; i < VECLEN*NUMTHREADS; ++i) {
    a[i] = 1;
    b[i] = a[i];
  }

  dotstr.veclen = VECLEN;
  dotstr.a = a;
  dotstr.b = b;
  dotstr.sum = 0;

  pthread_mutex_init(&mutexsum, NULL);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (i = 0; i < NUMTHREADS; ++i) {
    pthread_create(&callThd[i], &attr, dotprod, (void*)i);
  }
  pthread_attr_destroy(&attr);

  for (i = 0; i < NUMTHREADS; ++i) {
    pthread_join(callThd[i], &status);
  }

  printf("Sum = %f \n", dotstr.sum);
  free(a);
  free(b);
  pthread_exit(NULL);
  return 0;
}
