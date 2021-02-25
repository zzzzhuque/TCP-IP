//
// Created by zhuque on 2021/2/9.
//
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int status;
  pid_t pid = fork();

  if (pid == 0) {
    return 3;        ////< @note 第一个子进程在这里就结束了
  } else {
    printf("Child PID: %d \n", pid);
    pid = fork();
    if (pid == 0) {  ///< @note 第二个子进程在这里就结束了/
      return 7;
    } else {
      printf("Child PID: %d \n", pid);
      wait(&status);
      if (WIFEXITED(status)) {
        printf("Child send one: %d \n", WEXITSTATUS(status));
      }

      wait(&status);
      if (WIFEXITED(status)) {
        printf("Child send two: %d \n", WEXITSTATUS(status));
      }
      sleep(30);
    }
  }
  return 0;
}