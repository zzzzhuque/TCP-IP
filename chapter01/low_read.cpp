//
// Created by zhuque on 2021/1/21.
//
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main() {
  int fd;
  char buf[BUF_SIZE];

  fd = open("data.txt", O_RDONLY);
  if (fd == -1) {
    error_handling("open() error!");
  }
  printf("file descriptor: %d \n", fd);

  if (read(fd, buf, sizeof(buf)) == -1) {
    error_handling("read() error!");
  }
  printf("file data: %s", buf);
  close(fd);
  return 0;
}