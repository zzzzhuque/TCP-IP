//
// Created by zhuque on 2021/2/18.
//
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>

#define BUF_SIZE 3

int main(int argc, char *argv[]) {
  int fd1, fd2; ////< @note 保存在fd1和fd2中的文件描述符
  int len;
  char buf[BUF_SIZE];

  fd1 = open("news.txt", O_RDONLY);
  fd2 = open("cpt.txt", O_WRONLY|O_CREAT|O_TRUNC);

  while ((len=read(fd1, buf, sizeof(buf))) > 0) {
    write(fd2, buf, len);
  }

  close(fd1);
  close(fd2);
  return 0;
}