//
// Created by zhuque on 2021/2/9.
//
#include <cstdio>
#include <unistd.h>
#include <csignal>

void timeout(int sig) {
  if (sig == SIGALRM) {
    puts("Time Out!");
  }
  alarm(2);
}

void  keycontrol(int sig) {
  if (sig == SIGINT) {
    puts("CTRL+C pressed");
  }
}

int main(int argc, char *argv[]) {
  signal(SIGALRM, timeout);
  signal(SIGINT, keycontrol);
  alarm(2);

  for (int i = 0; i < 3; ++i) {
    puts("wait...");
    sleep(300);
  }
  return 0;
}