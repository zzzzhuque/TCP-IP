//
// Created by zhuque on 2021/2/22.
//
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100

long long num = 0;

void* thread_inc(void *arg) {
  for (int i = 0; i < 50000000; ++i) {
    num += 1;
  }
  return nullptr;
}

void* thread_des(void *arg) {
  for (int i = 0; i < 50000000; ++i) {
    num -= 1;
  }
  return nullptr;
}

int main(int argc, char *argv[]) {
  pthread_t thread_id[NUM_THREAD];

  printf("sizeof long long: %d \n", sizeof(long long));
  for (int i = 0; i < NUM_THREAD; ++i) {
    if (i % 2) {
      pthread_create(&(thread_id[i]), nullptr, thread_inc, nullptr);
    } else {
      pthread_create(&(thread_id[i]), nullptr, thread_des, nullptr);
    }
  }

  for (int j = 0; j < NUM_THREAD; ++j) {
    pthread_join(thread_id[j], nullptr);
  }
  printf("result: %lld\n", num);
  return 0;
}
