//
// Created by zhuque on 2021/2/3.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int serv_sock;
  char message[BUF_SIZE];
  int str_len;
  socklen_t clnt_adr_sz;

  char msg1[] = "ccc";
  char msg2[] = "ddd";

  sockaddr_in serv_adr, clnt_adr;
  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (serv_sock == -1) {
    error_handling("socket() error");
  }

  memset(&clnt_adr, 0, sizeof(clnt_adr));
  clnt_adr.sin_family = AF_INET;
  clnt_adr.sin_addr.s_addr = inet_addr(argv[1]);
  clnt_adr.sin_port = htons(atoi(argv[2]));

  ////< @attention 同时绑定本地和目标的IP和port
  connect(serv_sock, (sockaddr*)&clnt_adr, sizeof(clnt_adr));

  ////< @details 第一次发和收
  write(serv_sock, msg1, strlen(msg1));
  str_len = read(serv_sock, message, sizeof(message)-1);  ////< @note -1存放\0
  message[str_len] = 0;
  printf("message1 from another UDP: %s \n", message);
  ////< @details 第二次发和收
  write(serv_sock, msg2, strlen(msg2));
  str_len = read(serv_sock, message, sizeof(message)-1);  ////< @note -1存放\0
  message[str_len] = 0;
  printf("message2 from another UDP: %s \n", message);

  close(serv_sock);
  return 0;
}