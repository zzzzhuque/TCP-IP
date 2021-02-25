//
// Created by zhuque on 2021/2/22.
//
#include <cstdio>
#include <pthread.h>

int sum = 0;

void* thread_summation(void *arg) {
  int start = ((int*)arg)[0];
  int end = ((int*)arg)[1];

  while (start <= end) {
    sum += start;
    ++start;
  }

  return nullptr;
}

int main(int argc, char *argv[]) {
  pthread_t id_t1, id_t2;
  int range1[] = {1, 5};
  int range2[] = {6, 10};

  pthread_create(&id_t1, nullptr, thread_summation, (void *)range1);
  pthread_create(&id_t2, nullptr, thread_summation, (void *)range2);

  pthread_join(id_t1, nullptr);
  pthread_join(id_t2, nullptr);
  printf("result: %d\n", sum);
  return 0;
}
