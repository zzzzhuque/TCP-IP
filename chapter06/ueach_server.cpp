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

  char msg1[] = "aaa";
  char msg2[] = "bbb";

  sockaddr_in serv_adr, clnt_adr;
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (serv_sock == -1) {
    error_handling("UDP socket creation error");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_ddr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("bind() error");
  } ////< @note 分配主机的地址和端口

  clnt_adr_sz = sizeof(clnt_adr);
  ////< @details 第一次收和发，这边先运行
  str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
                     (sockaddr*)&clnt_adr, &clnt_adr_sz);
  message[str_len] = 0;
  printf("message1 from another UDP: %s \n", message);
  sendto(serv_sock, msg1, strlen(msg1), 0,
                     (sockaddr*)&clnt_adr, clnt_adr_sz);
  ////< @details 第二次收和发
  str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
                     (sockaddr*)&clnt_adr, &clnt_adr_sz);
  message[str_len] = 0;
  printf("message2 from another UDP: %s \n", message);
  sendto(serv_sock, msg2, strlen(msg2), 0,
         (sockaddr*)&clnt_adr, clnt_adr_sz);

  close(serv_sock);
  return 0;
}