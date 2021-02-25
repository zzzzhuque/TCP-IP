//
// Created by zhuque on 2021/2/19.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int sock;
  char buf[BUF_SIZE];
  sockaddr_in serv_addr;

  FILE *readfp, *writefp;
  sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
  readfp = fdopen(sock, "r");
  writefp = fdopen(sock, "w");

  while (true) {
    if (fgets(buf, sizeof(buf), readfp) == nullptr) {
      break;
    }
    fputs(buf, stdout);
    fflush(stdout);
  }
  fputs("FROM Client: Thank you! \n", writefp);
  fflush(writefp);
  fclose(readfp);
  fclose(writefp);
  return 0;
}