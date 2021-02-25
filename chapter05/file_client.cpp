//
// Created by zhuque on 2021/1/28.
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
#define FLAG_SIZE 1

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  char buff[BUF_SIZE];
  char filename[FILENAME_SIZE];
  char flag;
  int str_len = 0;
  sockaddr_in serv_adr;

  if (argc != 4) {
    printf("usage : %s <IP> <port> <filename>\n", argv[0]);
    exit(0);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0); ////< @note 协议族
  if (sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET; ////< @note 协议族
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  strcpy(filename, argv[3]);  ////< @note 获取文件名

  if (connect(sock, (sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("connect() error");
  } else {
    puts("connected......");
  }

  ////< @note write的长度不准确也没关系，但要保证后续的内容都是\0
  write(sock, filename, sizeof(filename));
  read(sock, &flag, 1);
  if (flag == '1') {
    int fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1) {
      error_handling("open() error!");
    }
    while ((str_len=read(sock, buff, 1000)) != 0) {
      if (write(fd, buff, str_len) == -1) {
        error_handling("write() error!");
      }
    }
    close(fd);
  }

  close(sock);
  return 0;
}