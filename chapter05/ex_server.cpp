//
// Created by zhuque on 2021/1/21.
//
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define LTH_SIZE 4

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int serv_sock;
  int clnt_sock;
  char serv_msg[] = "Hello?";
  char omsg[BUF_SIZE];
  char imsg[BUF_SIZE];

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

  for (int i = 0; i < 3; ++i) {
    *(int*)&omsg[0] = sizeof(serv_msg);
    strcpy(&omsg[LTH_SIZE], serv_msg);
    write(clnt_sock, omsg, sizeof(serv_msg)+LTH_SIZE);
  }

  for (int j = 0; j < 3; ++j) {
    int length = 0;
    read(clnt_sock, &length, sizeof(length));
    read(clnt_sock, imsg, length);
    imsg[length] = 0;
    printf("msg frm client: %s\n", imsg);
  }

  close(clnt_sock);
  close(serv_sock);
  return 0;
}