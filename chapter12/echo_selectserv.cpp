//
// Created by zhuque on 2021/2/13.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctime>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int serv_sock, clnt_sock;
  sockaddr_in serv_adr, clnt_adr;
  timeval timeout;
  fd_set reads, cpy_reads;

  socklen_t adr_sz;
  int fd_max, str_len, fd_num;
  char buf[BUF_SIZE];
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  FD_ZERO(&reads);
  FD_SET(serv_sock, &reads);
  fd_max = serv_sock;

  while (true) {
    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    if ((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1) {
      break;  ////< @note 监控失败，直接退出
    }
    if (fd_num == 0) {  ////< @note 监视的套接字没有变化
      continue;
    }
    for (int i = 0; i < fd_max+1; ++i) {
      if (FD_ISSET(i, &cpy_reads)) {
        if (i == serv_sock) { ////< @note 处理连接请求
          adr_sz = sizeof(clnt_adr);
          clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &adr_sz);
          FD_SET(clnt_sock, &reads);
          if (fd_max < clnt_sock) {
            fd_max = clnt_sock;
          }
          printf("connected client: %d\n", clnt_sock);
        } else {  ////< @note 读取信息
          str_len = read(i, buf, BUF_SIZE);
          if (str_len == 0) { ////< @note 没有信息，关闭连接
            FD_CLR(i, &reads);
            close(i);
            printf("closed client: %d\n", i);
          } else {  ////< @note 回传信息
            write(i, buf, str_len);
            sleep(5); ////< @note 提供服务
          }
        }
      }
    }
  }
  close(serv_sock);
  return 0;
}