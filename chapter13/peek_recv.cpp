//
// Created by zhuque on 2021/2/16.
//
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int acpt_sock, recv_sock;
  sockaddr_in acpt_adr, recv_adr;
  int str_len, state;
  socklen_t recv_adr_sz;
  char buf[BUF_SIZE];
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&acpt_adr, 0, sizeof(acpt_adr));
  acpt_adr.sin_family = AF_INET;
  acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  acpt_adr.sin_port = htons(atoi(argv[1]));

  if (bind(acpt_sock, (sockaddr*)&acpt_adr, sizeof(acpt_adr)) == -1) {
    error_handling("bind() error");
  }
  listen(acpt_sock, 5);

  recv_adr_sz = sizeof(recv_adr);
  recv_sock = accept(acpt_sock, (sockaddr*)&recv_adr, &recv_adr_sz);

  while (true) {
    str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK|MSG_DONTWAIT);
    if (str_len > 0) {  ////< @note 读到内容之后停止读取
      break;
    }
  }
  buf[str_len] = 0;
  printf("Buffering %d bytes: %s\n", str_len, buf);

  str_len = recv(recv_sock, buf, sizeof(buf)-1, 0);
  buf[str_len] = 0;
  printf("Read again: %s \n", buf);
  close(acpt_sock);
  close(recv_sock);
  return 0;
}