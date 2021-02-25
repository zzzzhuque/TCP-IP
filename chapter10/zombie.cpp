//
// Created by zhuque on 2021/2/8.
//
#include <cstdio>
#include <unistd.h>

int main(int argc, char *argv[]) {
  pid_t pid = fork();

  if (pid == 0) {
    puts("Hi, I am a child process");
  } else {
    printf("Child Process ID: %d\n", pid);
    sleep(30);
  }

  if (pid == 0) {
    puts("end child process");
  } else {
    puts("end parent process");
  }
  return 0;
}