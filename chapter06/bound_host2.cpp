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
  int sock;
  char msg1[] = "Hi!";
  char msg2[] = "I'm another UDP host!";
  char msg3[] = "Nice to meet you!";

  sockaddr_in your_adr;
  socklen_t your_adr_sz;
  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    error_handling("UDP socket creation error");
  }

  memset(&your_adr, 0, sizeof(your_adr));
  your_adr.sin_family = AF_INET;
  your_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  your_adr.sin_port = htons(atoi(argv[1]));

  sendto(sock, msg1, sizeof(msg1), 0,
         (sockaddr*)&your_adr, sizeof(your_adr));
  sendto(sock, msg2, sizeof(msg2), 0,
         (sockaddr*)&your_adr, sizeof(your_adr));
  sendto(sock, msg3, sizeof(msg3), 0,
         (sockaddr*)&your_adr, sizeof(your_adr));
  close(sock);
  return 0;
}