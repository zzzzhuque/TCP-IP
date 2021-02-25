//
// Created by zhuque on 2021/1/28.
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
  int serv_sock, clnt_sock;
  char message[] = "iter";
  int str_len;

  sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc != 2) {
    printf("usage: %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1) {
    error_handling("socket() error");
    exit(0);
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  clnt_adr_sz = sizeof(clnt_adr);

  for (int i = 0; i < 5; ++i) {
    clnt_sock = accept(serv_sock, (sockaddr *)&clnt_adr, &clnt_adr_sz);
    if (clnt_sock == -1) {
      error_handling("accept() error");
    } else {
      printf("connected client %d \n", i + 1);
    }

    write(clnt_sock, message, sizeof(message));

    close(clnt_sock);
  }
  close(serv_sock);
  return 0;
}