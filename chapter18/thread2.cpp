//
// Created by zhuque on 2021/2/22.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

void* thread_main(void *arg) {
  int cnt = *((int*)arg);
  char *msg = (char*)malloc(sizeof(char)*50);
  strcpy(msg, "Hello, I'am thread~\n");

  for (int i = 0; i < cnt; ++i) {
    sleep(1);
    puts("running thread");
  }
  return (void*)msg;
}

int main(int argc, char *argv[]) {
  pthread_t t_id;
  int thread_param = 5;
  void *thr_ret = nullptr;

  if (pthread_create(&t_id, nullptr, thread_main, (void*)&thread_param) != 0) {
    puts("pthread_create() error");
    return -1;
  }

  if (pthread_join(t_id, &thr_ret) != 0) {
    puts("pthread_join() error");
    return -1;
  }

  printf("thread return message: %s \n", (char*)thr_ret);
  free(thr_ret);
  return 0;
}