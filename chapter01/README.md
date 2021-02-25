1、套接字在网络编程中的作用是什么？为何称它为套接字？

套接字是网络数据传输用的软件设备。socket的意思是插座，
本身就含有连接的意思，翻译成套接字表示两台计算机在网络中的连接

2、在服务器端创建套接字后，会依次调用listen函数和accept函数。请比较说明二者的作用

拿电话机来作比方，listen函数就是连接电话线这一步，连接了电话线表示
套接字状态转变为可接听状态。
accept函数相当于电话铃响了，拿起电话，表示受理连接请求。

3、Linux中，对套接字数据进行I/O时可以直接使用文件I/O相关函数；
而在Windows中则不可以，原因为何？

仅回答前半个问题。因为在linux系统中将socket也看作一种文件。

4、创建套接字后一般会给它分配地址，为什么？为了完成地址分配需要调用哪个函数？

先回答后面的问题，调用`bind()`函数完成地址分配。再回答前面的问题，服务端和
客户端分配地址的方式不同。服务端需要指定端口，客户端需要指定IP地址和端口。

5、Linux中的文件描述符与windows的句柄实际上非常相似。请以套接字为对象说明
它们的含义。

略

6、底层文件I/O函数与ANSI标准定义的文件I/O函数之间有何区别？

ANSI标准全称是American National Standards Institute
Linux提供的底层文件I/O函数有`open() close() read() write()`。ANSI标准
定义的文件I/O函数，又称为标准文件I/O函数，包含`fopen() fclose() fread() 
fwrite()`。

底层文件I/O又称为低级磁盘I/O，遵循POSIX(Portable Operating System Interface)
相关标准。任何兼容POSIX标准的操作系统上都支持底层文件I/O。标准文件I/O被称为高级磁
盘I/O，遵循ANSI C相关标准。只要开发环境中有标准文件I/O库，标准文件I/O就可以使用。
(Linux 中使用的是GLIBC，它是标准C库的超集。不仅包含ANSI C中定义的函数，还包括POSIX
标准中定义的函数。因此，Linux 下既可以使用标准文件I/O，也可以使用底层文件I/O)。

通过底层文件I/O读写文件时，每次操作都会**执行相关系统调用**。这样处理的好处是直接读
写实际文件，坏处是频繁的系统调用会增加系统开销。

标准文件I/O可以看成是在底层文件I/O的基础上**封装了缓冲机制**。先读写缓冲区，必要时
再访问实际文件，从而减少了系统调用的次数。

底层文件I/O中用文件描述符表现一个打开的文件，可以访问不同类型的文件如普通文件、设备
文件和管道文件等。而标准I/O中用FILE（流）表示一个打开的文件，通常只用来访问普通文件。

7、参考本书给出的示例`low_open.c`和`low_read.c`，分别利用底层文件I/O和ANSI标准I/O
编写文件复制程序。可任意指定复制程序的使用方法。

```c++
//
// Created by zhuque on 2021/1/21.
//
// ansi_open.cpp
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main() {
  char buf[] = "Let's go!\n";
  FILE *fd = std::fopen("fdata.txt", "w");
  if (fd == nullptr) {
    error_handling("open() error!");
  }

  if (std::fwrite(buf, sizeof(buf), 1, fd) != 1) {
    error_handling("write() error!");
  }
  std::fclose(fd);
  return 0;
}
```
mac上的表现：
这是使用标准文件I/O函数改写的`low_open.cpp`，一个显著的区别是文件的权限，使用底
层文件I/O函数创建的文件权限为000，而使用标准文件I/O函数创建的文件权限则为644。甚
至，当这个权限为000的文件存在时再次`./low_open.o`会报错，文件打开错误。

linux上的表现：
使用底层文件I/O函数创建的文件权限为430，而使用标准文件I/O函数创建的文件权限则为
664。甚至，当这个权限为430的文件存在时再次`./low_open.o`会报错，文件打开错误。

但在文件复制功能上，两者表面上没有区别。