#include <stdio.h>
#include <pthread.h>

#define N_THREADS 5

static int exited;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void thr_exit() {
  pthread_mutex_lock(&mutex);

  /* YOUR CODE HERE */

  pthread_mutex_unlock(&mutex);
}

void thr_join() {
  pthread_mutex_lock(&mutex);

  while (exited < N_THREADS) /* YOUR CODE HERE */

  pthread_mutex_unlock(&mutex);
}

void *child_func(void *arg) {
  int thr_id = /* YOUR CODE HERE */;
  printf("child %d created and exited\n", thr_id);
  thr_exit();
  return NULL;
}

int main() {
  pthread_t p[N_THREADS];
  int thr_idx[N_THREADS];
  void *arg;
  int i;

  exited = /* YOUR CODE HERE */;

  puts("parent: begin");

  for (i = 0; i < N_THREADS; i++) {
    thr_idx[i] = i;
    arg = &thr_idx[i];

    pthread_create(arg,NULL,child_func,NULL);
  }

  thr_join();

  puts("parent: end");

  return 0;
}
