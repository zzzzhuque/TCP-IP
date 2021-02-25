//
// Created by zhuque on 2021/2/16.
//
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  sockaddr_in send_adr;
  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&send_adr, 0, sizeof(send_adr));
  send_adr.sin_family = AF_INET;
  send_adr.sin_addr.s_addr = inet_addr(argv[1]);
  send_adr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (sockaddr*)&send_adr, sizeof(send_adr)) == -1) {
    error_handling("connect() error");
  }

  write(sock, "123", strlen("123"));
  close(sock);
  return 0;
}