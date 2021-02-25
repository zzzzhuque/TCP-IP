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

char msg[BUF_SIZE] = {0,};
pthread_mutex_t mtx_for_msg;

void* handle_clnt(void *arg) {
  int clnt_sock = *((int*)arg);
  int str_len = 0;
  // char msg[BUF_SIZE];

  printf("I want mtx_for_msg\n");
  pthread_mutex_lock(&mtx_for_msg);
  printf("I got mtx_for_msg\n");
  while ((str_len=read(clnt_sock, msg, sizeof(msg))) != 0) {
    msg[str_len] = '\0';
    write(clnt_sock, msg, str_len);
  }
  pthread_mutex_unlock(&mtx_for_msg);

  close(clnt_sock);
  printf("closed client socket: %d \n", clnt_sock);
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

  int option;
  socklen_t optlen;

  if (argc != 2) {
    printf("Uasge: %s <port> \n", argv[0]);
    exit(1);
  }

  pthread_mutex_init(&mtx_for_msg, nullptr);
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  optlen = sizeof(option);
  option = 1;
  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
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

    pthread_create(&t_id, nullptr, handle_clnt, (void*)&clnt_sock);
    pthread_detach(t_id);
    printf("connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
  }
  close(serv_sock);
  return 0;
}