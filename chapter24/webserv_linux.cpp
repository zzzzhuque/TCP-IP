//
// Created by zhuque on 2021/2/24.
//
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

void send_error(FILE *fp) {
  char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
  char server[]   = "Server:Linux Web Server \r\n";
  char cnt_len[]  = "Content-length:2048\r\n";
  char cnt_type[] = "Content-type:text/html\r\n\r\n";
  char content[]  = "<html><head><title>NETWORK</title></head>"
                       "<body><font size=+5><br>error!please check file name or request type!"
                       "</font></body></html>";

  fputs(protocol, fp);
  fputs(server,   fp);
  fputs(cnt_len,  fp);
  fputs(cnt_type, fp);
  fputs(content,  fp);
  fflush(fp);
}

const char* content_type(char* file) {
  char extension[SMALL_BUF];                ////< @note 文件扩展名
  char file_name[SMALL_BUF];                ////< @note 文件名
  strcpy(file_name, file);
  strtok(file_name, ".");                   ////< @note 将file_name中"."替换为'\0'
  strcpy(extension, strtok(nullptr, "."));  ////< @note 传入nullptr表示继续分割file_name

  if (!strcmp(extension, "html") || !strcmp(extension, "htm")) {
    return "text/html";
  } else {
    return "text/plain";
  }
}

void send_data(FILE *fp, char *ct, char *file_name) {
  char protocol[] = "HTTP/1.0 200 OK\r\n";
  char server[]   = "Server:Linux Web Server \r\n";
  char cnt_len[]  = "Content-length:2048\r\n";
  char cnt_type[SMALL_BUF];
  char buf[BUF_SIZE];
  FILE *send_file;

  sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
  send_file = fopen(file_name, "r");
  if (send_file == nullptr) {             ////< @note 未找到该文件，向套接字描述符写入错误信息
    send_error(fp);
    return;
  }

  fputs(protocol, fp);                    ////< @note 传输头信息
  fputs(server,   fp);
  fputs(cnt_len,  fp);
  fputs(cnt_type, fp);

  while (fgets(buf, BUF_SIZE, send_file) != nullptr) {
    fputs(buf, fp);                       ////< @note 传输请求数据
    fputs(buf, stdout);
    fflush(fp);
  }
  fflush(fp);
  fclose(fp);                             ////< @note 关闭客户端套接字，发送EOF
}

void* request_handler(void *arg) {
  int clnt_sock = *((int*)arg);
  char req_line[SMALL_BUF];               ////< @note 存放请求行
  FILE *clnt_read, *clnt_write;

  char method[10];
  char ct[15];
  char file_name[30];

  clnt_read = fdopen(clnt_sock, "r");     ////< @note 转换为FILE结构体，用于读取
  clnt_write = fdopen(dup(clnt_sock), "w");       ////< @note 复制套接字描述符，用于写入
  fgets(req_line, SMALL_BUF, clnt_read);  ////< @note 获取请求行
  if (strstr(req_line, "HTTP/") == nullptr) {     ////< @note 发生错误，返回400
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return nullptr;
  }

  strcpy(method, strtok(req_line, " /"));         ////< @note 从请求行截取请求方式
  strcpy(file_name, strtok(nullptr, " /"));       ////< @note 从请求行截取请求文件名
  strcpy(ct, content_type(file_name));            ////< @note 获取请求文件的类型
  if (strcmp(method, "GET") != 0) {               ////< @note 发生错误，返回400
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return nullptr;
  }

  fclose(clnt_read);                        ////< @attention 仅关闭用于读取的套接字描述符，且不发送EOF
  send_data(clnt_write, ct, file_name);     ////< @note 在`send_data`函数中关闭clnt_write
}

int main(int argc, char *argv[]) {
  int serv_sock, clnt_sock;
  int option;
  socklen_t optlen;
  sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_size;
  char buf[BUF_SIZE];
  pthread_t t_id;
  if (argc != 2) {
    printf("Uasge: %s <port>\n", argv[0]);
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

  while (true) {
    clnt_adr_size = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &clnt_adr_size);
    printf("Connection Request : %s:%d\n",
           inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
    pthread_create(&t_id, nullptr, request_handler, &clnt_sock);
    pthread_detach(t_id);
  }
  close(serv_sock);
  return 0;
}