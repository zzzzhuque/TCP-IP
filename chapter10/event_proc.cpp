//
// Created by zhuque on 2021/2/11.
//
#include <cstdio>
#include <csignal>
#include <unistd.h>

void processor(int sig) {
  char ch = '\0';
  if (sig == SIGINT) {
    puts("input Y to quit");
  }
  ch = getchar();
  getchar();  ////< @note 吃掉换行符
  if (ch == 'Y') {
    exit(1);
  }
  return;
}

int main(int argc, char *argv[]) {
  char msg[] = "hello";

  struct sigaction act;
  act.sa_handler = processor;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGINT, &act, 0);

  while (true) {
    sleep(1);
    printf("%s\n", msg);
  }
  return 0;
}