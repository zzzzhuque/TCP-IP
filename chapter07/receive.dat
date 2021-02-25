//
// Created by zhuque on 2021/2/6.
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
  int serv_sd, clnt_sd;
  FILE *fp;
  char buf[BUF_SIZE];
  int read_cnt;

  sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  fp = fopen("file_server.cpp", "rb");
  serv_sd = socket(PF_INET, SOCK_STREAM, 0);

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  bind(serv_sd, (sockaddr*)&serv_adr, sizeof(serv_adr));
  listen(serv_sd, 5);

  clnt_adr_sz = sizeof(clnt_adr);
  clnt_sd = accept(serv_sd, (sockaddr*)&clnt_adr, &clnt_adr_sz);

  while (true) {
    read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
    if (read_cnt < BUF_SIZE) {  ////< @note 说明读完了
      write(clnt_sd, buf, read_cnt);
      break;
    }
    write(clnt_sd, buf, BUF_SIZE);
  }

  shutdown(clnt_sd, SHUT_WR);
  read(clnt_sd, buf, BUF_SIZE);
  printf("message from client: %s\n", buf);

  fclose(fp);
  close(clnt_sd);
  close(serv_sd);
  return 0;
}