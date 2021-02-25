//
// Created by zhuque on 2021/2/20.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]){
  int serv_sock, clnt_sock;
  sockaddr_in serv_adr, clnt_adr;
  socklen_t adr_sz;
  int str_len;
  char buf[BUF_SIZE];

  epoll_event *ep_events;
  epoll_event event;
  int epfd, event_cnt;

  if (argc != 2) {
    printf("Usage: %s <port> \n", argv[0]);
    exit(1);
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

  epfd = epoll_create(EPOLL_SIZE);      ////< @note 创建epoll例程
  ep_events = (epoll_event*)malloc(sizeof(epoll_event)*EPOLL_SIZE);   ////< @note 存放发生事件的epoll_event

  event.events = EPOLLIN;               ////< @note event是epoll_event类型的变量
  event.data.fd = serv_sock;            ////< @note epoll_event中包含serv_sock
  epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);    ////< @note 将epoll_event注册到epoll例程中

  while (true) {
    event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
    if (event_cnt == -1) {
      puts("epoll_wait error");
      break;                            ////< @note 不能调用error_handling，还要关闭套接字
    }

    for (int i = 0; i < event_cnt; ++i) {
      if (ep_events[i].data.fd == serv_sock) {          ////< @note 有新的连接产生
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &adr_sz);
        event.events = EPOLLIN;
        event.data.fd = clnt_sock;
        epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
        printf("connected client: %d \n", clnt_sock);
      } else {                          ////< @note 客户端发送消息到服务器端
        str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
        if (str_len == 0) {             ////< @note 客户端发送EOF，准备关闭连接
          epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, nullptr);
          close(ep_events[i].data.fd);
          printf("closed client: %d \n", ep_events[i].data.fd);
        } else {                        ////< @note 将消息回复给客户端
          write(ep_events[i].data.fd, buf, str_len);
        }
      }
    }
  }
  close(serv_sock);
  close(epfd);
  return 0;
}