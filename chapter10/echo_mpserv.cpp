//
// Created by zhuque on 2021/2/10.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void read_childproc(int sig) {
  int status;
  pid_t id = waitpid(-1, &status, WNOHANG);
  if (WIFEXITED(status)) {
    printf("Removed proc id: %d \n", id);
  }
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int serv_sock, clnt_sock;
  sockaddr_in serv_adr, clnt_adr;

  pid_t pid;
  struct sigaction act;
  socklen_t adr_sz;
  int str_len, state;
  char buf[BUF_SIZE];
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  act.sa_handler = read_childproc;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  state = sigaction(SIGCHLD, &act, 0);

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr))==-1) {
    error_handling("bind() error");
  }
  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  while (true) {
    adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &adr_sz);
    if (clnt_sock == -1) {
      continue;
    } else {
      puts("new client connected...");
    }
    pid = fork();
    if (pid == -1) {  ////< @note 进程创建失败
      close(clnt_sock);
      continue;
    }
    if (pid == 0) {
      close(serv_sock); ////< @note 子进程关闭服务器端套接字文件描述符
      while ((str_len=read(clnt_sock, buf, BUF_SIZE)) != 0) {
        printf("read %d char from sock %d\n", str_len, clnt_sock);
        write(clnt_sock, buf, str_len);
      }
      close(clnt_sock);
      puts("client disconnected...");
      return 0;
    } else {
      close(clnt_sock); ////< @note 父进程关闭客户端套接字文件描述符
    }
  }
  close(serv_sock);
  return 0;
}