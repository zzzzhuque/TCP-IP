//
// Created by zhuque on 2021/2/17.
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
  int send_sock;
  sockaddr_in broad_adr;
  FILE *fp;
  char buf[BUF_SIZE];
  int so_brd = 1;
  if (argc != 3) {
    printf("Uasge: %s <Boardcast IP> <PORT>\n", argv[0]);
    exit(1);
  }

  send_sock = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&broad_adr, 0, sizeof(broad_adr));
  broad_adr.sin_family = AF_INET;
  broad_adr.sin_addr.s_addr = inet_addr(argv[1]); ////< @note 多播IP
  broad_adr.sin_port = htons(atoi(argv[2]));   ////< @note 多播端口

  setsockopt(send_sock, SOL_SOCKET,
             SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
  if ((fp=std::fopen("news.txt", "r")) == nullptr) {
    error_handling("fopen() error");
  }

  while (!feof(fp)) { ////< @note 开始多播，直到文件数据传输完毕
    fgets(buf, BUF_SIZE, fp);
    printf("multicast message: %s\n", buf);
    sendto(send_sock, buf, strlen(buf), 0, (sockaddr *)&broad_adr,
           sizeof(broad_adr));
    sleep(2);
  }
  fclose(fp);
  close(send_sock);
  return 0;
}