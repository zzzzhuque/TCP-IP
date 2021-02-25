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
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  char opmsg[BUF_SIZE];
  int result, opnd_cnt;
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

  fputs("Operand count: ", stdout);
  scanf("%d", &opnd_cnt);
  opmsg[0] = (char)opnd_cnt;

  for (int i = 0; i < opnd_cnt; ++i) {
    printf("Operand %d: ", i+1);
    scanf("%d", (int*)&opmsg[i*OPSZ+1]);
  }
  fgetc(stdin); ////< @note 读取'\n'
  fputs("Operator: ", stdout);
  scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
  write(sock, opmsg, opnd_cnt*OPSZ+2);
  read(sock, &result, RLT_SIZE);

  printf("Operation result: %d \n", result);
  close(sock);
  return 0;
}