//
// Created by zhuque on 2021/1/25.
//
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main() {
  const char *addr = "127.232.124.79";
  sockaddr_in addr_inet;

  if (!inet_aton(addr, &addr_inet.sin_addr)) {
    error_handling("conversion error");
  } else {
    printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
  }
  return 0;
}