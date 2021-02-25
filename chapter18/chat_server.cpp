//
// Created by zhuque on 2021/2/24.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>
#include <cerrno>

#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

void send_msg(char *msg, int len) {         ////< @note 向所有连接的客户端发送消息
  pthread_mutex_lock(&mutx);
  for (int i = 0; i < clnt_cnt; ++i) {
    write(clnt_socks[i], msg, len);
  }
  pthread_mutex_unlock(&mutx);
}

void* handle_clnt(void *arg) {
  int clnt_sock = *((int*)arg);
  int str_len = 0;
  char msg[BUF_SIZE];

  while ((str_len=read(clnt_sock, msg, sizeof(msg))) != 0) {
    send_msg(msg, str_len);
  }

  pthread_mutex_lock(&mutx);
  for (int i = 0; i < clnt_cnt; ++i) {      ////< @note 移除客户端
    if (clnt_sock == clnt_socks[i]) {
      while (i++ < clnt_cnt-1) {
        clnt_socks[i] = clnt_socks[i+1];
      }
      break;
    }
  }
  --clnt_cnt;
  pthread_mutex_unlock(&mutx);
  close(clnt_sock);
  return nullptr;
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  int serv_sock, clnt_sock;
  sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;
  pthread_t t_id;

  if (argc != 2) {
    printf("Uasge: %s <port> \n", argv[0]);
    exit(1);
  }

  pthread_mutex_init(&mutx, nullptr);
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

  while (true) {
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &clnt_adr_sz);

    pthread_mutex_lock(&mutx);
    clnt_socks[clnt_cnt++] = clnt_sock;
    pthread_mutex_unlock(&mutx);

    pthread_create(&t_id, nullptr, handle_clnt, (void*)&clnt_sock);
    pthread_detach(t_id);
    printf("connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
  }
  close(serv_sock);
  return 0;
}