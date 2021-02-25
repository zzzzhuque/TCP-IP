//
// Created by zhuque on 2021/1/21.
//
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define FILENAME_SIZE 32

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int serv_sock;
  int clnt_sock;
  int str_len = 0;
  char buff[BUF_SIZE];
  char filename[FILENAME_SIZE];

  sockaddr_in serv_addr;
  sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;

  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serv_sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  clnt_addr_size = sizeof(clnt_addr);
  clnt_sock = accept(serv_sock, (sockaddr *)&clnt_addr, &clnt_addr_size);
  if (clnt_sock == -1) {
    error_handling("accept() error");
  }

  if (read(clnt_sock, filename, FILENAME_SIZE) != -1) {
    printf("from client filename: %s\n", filename);
  } else {
    error_handling("read() error");
  }
  int fd = open(filename, O_RDONLY);
  char flag = '0';
  if (fd == -1) {
    write(clnt_sock, &flag, 1);
  } else {
    flag = '1';
    write(clnt_sock, &flag, 1);
    while ((str_len=read(fd, buff, 1000)) != 0) {
      write(clnt_sock, buff, str_len);
    }
    close(fd);
  }

  close(clnt_sock); ////< @note 给客户端的输入缓冲一个EOF，告诉客户端传输结束了
  close(serv_sock);
  return 0;
}