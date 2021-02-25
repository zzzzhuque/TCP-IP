//
// Created by zhuque on 2021/2/11.
//
#include <cstdio>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
  int fds[2];
  char str[] = "Who are you?";
  char buff[BUF_SIZE];
  pid_t pid;

  pipe(fds);
  pid = fork();
  if (pid == 0) {
    write(fds[1], str, sizeof(str));
  } else {
    read(fds[0], buff, BUF_SIZE);
    puts(buff);
  }
  return 0;
}