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
#define LTH_SIZE 4

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  char clnt_msg[] = "Yes";
  char omsg[BUF_SIZE];
  char imsg[BUF_SIZE];
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

  for (int i = 0; i < 3; ++i) {
    int length = 0;
    read(sock, &length, sizeof(length));
    read(sock, imsg, length);
    imsg[length] = 0;
    printf("msg frm server: %s\n", imsg);
  }

  for (int j = 0; j < 3; ++j) {
    *(int*)&omsg[0] = sizeof(clnt_msg);
    strcpy(&omsg[LTH_SIZE], clnt_msg);
    write(sock, omsg, sizeof(clnt_msg)+LTH_SIZE);
  }

  close(sock);
  return 0;
}