//
// Created by zhuque on 2021/2/6.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  hostent *host;
  sockaddr_in addr;
  if (argc != 2) {
    printf("Uasge: %s <IP>\n", argv[0]);
    exit(1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  host = gethostbyaddr((char*)&(addr.sin_addr), 4, AF_INET);
  if (!host) {
    error_handling("get host error");
  }

  printf("Official name: %s \n", host->h_name);
  for (int i = 0; host->h_aliases[i] ; ++i) {
    printf("Aliases %d: %s\n", i+1, host->h_aliases[i]);
  }
  printf("Address type: %s \n", (host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
  for (int i = 0; host->h_addr_list[i] ; ++i) {
    printf("IP addr %d: %s\n", i+1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));
  }
  return 0;
}