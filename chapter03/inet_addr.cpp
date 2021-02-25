//
// Created by zhuque on 2021/1/25.
//
#include <arpa/inet.h>
#include <cstdio>

int main() {
  const char *addr1 = "1.2.3.4";
  const char *addr2 = "1.2.3.256";

  unsigned long conv_addr = inet_addr(addr1);
  if (conv_addr == INADDR_NONE) {
    printf("Error occured\n");
  } else {
    printf("Network ordered integer addr: %#lx \n", conv_addr);
  }

  conv_addr = inet_addr(addr2);
  if (conv_addr == INADDR_NONE) {
    printf("Error occured\n");
  } else {
    printf("Network ordered integer addr: %#lx \n\n", conv_addr);
  }
  return 0;
}