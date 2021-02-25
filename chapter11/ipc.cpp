//
// Created by zhuque on 2021/2/12.
//
#include <cstdio>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
  int fds1[2], fds2[2];
  const char *msg[3] = {"111", "222", "333"};
  char buf[BUF_SIZE];
  pid_t pid;

  pipe(fds1); pipe(fds2);
  pid = fork();
  if (pid == 0) { ////< @note 子进程利用fds1向父进程传输消息，利用fds2从父进程读取消息
    for (int i = 0; i < 3; ++i) {
      write(fds1[1], msg[i], sizeof(msg[i]));
      read(fds2[0], buf, BUF_SIZE);
      printf("Child proc output: %s\n", buf);
    }
  } else {
    for (int i = 0; i < 3; ++i) {
      read(fds1[0], buf, BUF_SIZE);
      printf("Parent proc output: %s\n", buf);
      write(fds2[1], msg[i], sizeof(msg[i]));
    }
    sleep(3);
  }
  return 0;
}