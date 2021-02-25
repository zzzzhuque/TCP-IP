//
// Created by zhuque on 2021/1/21.
//
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  sockaddr_in serv_addr;
  char message[30];
  int str_len = 0;
  int idx = 0, read_len = 0;

  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("connect() error");
  }

  /*
  while (read_len =
             read(sock, &message[idx++], 1)) { ////< @note 每次读取一个字节
    if (read_len == -1) {
      error_handling("read() error");
    }
    str_len += read_len;
  }
  printf("function read call count: %d \n", str_len);
   */
  ////< @note 习题6
  for (int i = 0; i < 3000; ++i) {
    printf("wait time %d \n", i);
  }
  read(sock, message, sizeof(message));

  printf("message from server : %s \n", message);
  close(sock);
  return 0;
}