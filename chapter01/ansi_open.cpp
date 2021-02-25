//
// Created by zhuque on 2021/1/21.
//
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main() {
  char buf[] = "Let's go!\n";
  FILE *fd = std::fopen("fdata.txt", "w");
  if (fd == nullptr) {
    error_handling("open() error!");
  }

  if (std::fwrite(buf, sizeof(buf), 1, fd) != 1) {
    error_handling("write() error!");
  }
  std::fclose(fd);
  return 0;
}