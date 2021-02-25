//
// Created by zhuque on 2021/2/11.
//
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  int sock;
  sock = socket(PF_INET, SOCK_STREAM, 0);

  pid_t pid = fork();
  if (pid == 0) { ////< @note 子进程
    printf("child proc sock fd: %d \n", sock);
  } else {
    printf("main proc sock fd: %d \n", sock);
  }
  close(sock);
  return 0;
}