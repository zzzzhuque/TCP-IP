//
// Created by zhuque on 2021/2/24.
//
#include <cstdio>
#include <pthread.h>
#include <semaphore.h>

static sem_t sem_one;
static sem_t sem_two;
static int num;

void* read(void *arg) {
  for (int i = 0; i < 5; ++i) {
    fputs("Input num: ", stdout);

    sem_wait(&sem_two);
    scanf("%d", &num);
    sem_post(&sem_one);
  }
  return nullptr;
}

void* accu(void *arg) {
  int sum = 0;
  for (int i = 0; i < 5; ++i) {
    sem_wait(&sem_one);
    sum += num;
    sem_post(&sem_two);
  }
  printf("result: %d\n", sum);
  return nullptr;
}

int main(int argc, char *argv[]) {
  pthread_t id_t1, id_t2;
  sem_init(&sem_one, 0, 0);
  sem_init(&sem_two, 0, 1);

  pthread_create(&id_t1, nullptr, read, nullptr);
  pthread_create(&id_t1, nullptr, accu, nullptr);

  pthread_join(id_t1, nullptr);
  pthread_join(id_t2, nullptr);

  sem_destroy(&sem_one);
  sem_destroy(&sem_two);
  return 0;
}