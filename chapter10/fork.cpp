//
// Created by zhuque on 2021/2/8.
//
#include <cstdio>
#include <unistd.h>

int gval = 10;

int main(int argc, char *argv[]) {
  pid_t pid;
  int lval = 20;
  gval++, lval += 5;

  pid = fork();
  if (pid == 0) { ////< @note if Child Process
    gval += 2, lval += 2;
  } else {        ////< @note if Parent Process
    gval -= 2, lval -= 2;
  }

  if (pid == 0) {
    printf("Child Proc: [%d, %d]\n", gval, lval);
  } else {
    printf("Parent Proc: [%d, %d]\n", gval, lval);
  }
  return 0;
}