//
// Created by zhuque on 2021/1/28.
//
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  char message[BUF_SIZE];
  int str_len;
  sockaddr_in serv_adr;

  if (argc != 3) {
    printf("usage : %s <IP> <port>\n", argv[0]);
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

  if (connect(sock, (sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("connect() error");
  } else {
    puts("connected......");
  }

  str_len = read(sock, message, BUF_SIZE - 1);
  message[str_len] = 0; ////< @note 结束符
  printf("message from server: %s", message);

  close(sock);
  return 0;
}