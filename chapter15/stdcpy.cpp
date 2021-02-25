//
// Created by zhuque on 2021/2/18.
//
#include <cstdio>

#define BUF_SIZE 3

int main(int argc, char *argv[]) {
  FILE *fp1, *fp2;
  char buf[BUF_SIZE];

  fp1 = fopen("news.txt", "r");
  fp2 = fopen("cpy.txt", "w");

  while (fgets(buf, BUF_SIZE, fp1) != nullptr) {
    fputs(buf, fp2);
  }

  fclose(fp1);
  fclose(fp2);
  return 0;
}