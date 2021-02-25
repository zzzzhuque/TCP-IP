//
// Created by zhuque on 2021/2/21.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <cerrno>
#include <vector>

using std::vector;

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void setnonblockingmode(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

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
  int option;
  socklen_t optlen;

  vector<int> clnt_socks;

  epoll_event *ep_events;
  epoll_event event;
  int epfd, event_cnt;
  if (argc != 2) {
    printf("Usage: %s <port> \n", argv[0]);
    exit(1);
  }

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

  epfd = epoll_create(EPOLL_SIZE);
  ep_events = (epoll_event*)malloc(sizeof(epoll_event)*EPOLL_SIZE);

  setnonblockingmode(serv_sock);
  event.events = EPOLLIN;
  event.data.fd = serv_sock;
  epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

  while (true) {
    event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
    if (event_cnt == -1) {
      puts("epoll_wait() error");
      break;
    }

    puts("return epoll_wait");
    for (int i = 0; i < event_cnt; ++i) {
      if (ep_events[i].data.fd == serv_sock) {      ////< @note 产生新的连接
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &adr_sz);
        clnt_socks.push_back(clnt_sock);
        setnonblockingmode(clnt_sock);
        event.events = EPOLLIN|EPOLLET;             ////< @note 边缘触发
        event.data.fd = clnt_sock;
        epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
        printf("connected client: %d \n", clnt_sock);
      } else {
        while (true) {
          str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
          if (str_len == 0) {                       ////< @note 收到EOF断开连接
            epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, nullptr);
            close(ep_events[i].data.fd);
            for (auto iter=clnt_socks.begin(); iter < clnt_socks.end(); ++iter) {
              if (*iter == ep_events[i].data.fd) {
                clnt_socks.erase(iter);
                break;
              }
            }
            printf("closed client: %d \n", ep_events[i].data.fd);
            break;
          } else if (str_len < 0) {                 ////< @note 缓冲区已经读取全部内容
            if (errno == EAGAIN) {
              printf("closed client: %d read failed\n", ep_events[i].data.fd);
              break;
            }
          } else {
            buf[str_len] = '\0';
            for (const auto fd : clnt_socks) {   ////< @note 把消息发送给所有客户端
              if (fd != serv_sock) {
                printf("echo msg to: %d \n", fd);
                write(fd, buf, BUF_SIZE);
              }
            }
          }
        }
      }
    }
  }
  close(serv_sock);
  close(epfd);
  return 0;
}