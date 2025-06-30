# Network_Programming

## 《TCP/IP网络编程》

### 第1章 理解网络编程和套接字
#### 1.1 网络编程中接受连接请求的套接字创建过程
1. 调用 socket 函数创建套接字
2. 调用 bind 函数分配 IP 地址和端口号
3. 调用 listen 函数将套接字转化为可接收请求状态
4. 调用 accept 函数受理套接字请求
#### 1.2 基于 Linux 的文件操作
Linux 中，socket被认为是文件的一种，因此在网络数据传输过程中，可以自然的使用I/O相关的函数。而Windows中，则区分socket和文件，需要使用特殊的数据传输函数。

##### 1.2.1 底层访问和文件描述符
分配给标准输入输出和标准错误的文件描述符
| 文件描述符 | 对象 |
| :-----: | :------:|
| 0 | 标准输入：Standard Input |
| 1 | 标准输出：Standard Output |
| 2 | 标准错误：Standard Error |

文件描述符也被称为 文件句柄，但 句柄 主要是 Windows 中的术语，Linux 中一般用 描述符。

##### 1.2.2 打开文件
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char* path, int flag);
/*
成功时返回文件描述符，失败时返回-1
path：文件名的字符串地址
flag：文件打开模式信息
*/
```
文件打开模式如下表：
| 打开模式 | 含义 |
| :-: | :-: |
| O_CREAT | 必要时创建文件 |
| O_TRUNC | 删除全部现有数据 |
| O_APPEND | 维持现有数据，追加到结尾 |
| O_RDONLY | 只读打开 |
| O_WRONLY | 只写打开 |
| O_RDWR | 读写打开 |

##### 1.2.3 关闭文件
```c
#include <unistd.h>
int close(int fd);
/*
成功时返回 0 ，失败时返回 -1
fd：要关闭的文件或套接字的文件描述符
```
此函数不仅可以关闭文件，还可以关闭套接字。进一步证明了Linux中不区分文件和套接字。

##### 1.2.4 将数据写入文件
```c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t nbytes);
/*
成功时返回写入的字节数，失败时返回-1
fd: 数据传输对象的文件描述符
buf: 要传输的数据的缓冲区地址
nbytes: 要传输的的字节数
*/
```
size_t 是通过 typedef 声明的 unsigned int 类型，ssize_t 前面多加的 s 表示 signed，即 ssize_t 是通过 typedef 声明的 signed int 类型。

##### 1.2.5. 读取文件中的数据
```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
/*
参数意义与write类似
*/
```

#### 1.3 习题
1. 套接字在网络编程中的作用是什么？为何称它为套接字？ 

    套接字在网络编程中的作用：
    1. 通信端点：套接字是网络通信的端点，每个套接字都有唯一的IP地址和端口号
    2. 数据传输：套接字支持数据的发送和接受，可以使用TCP或UDP协议
    3. 连接管理：负责网络连接的建立、维护和终止
    4. 编程接口：提供标准化的编程接口，用于建立和管理网络连接
    
    套接字名称的由来：
    因为它类似于电气工程中的“插座”，是网络连接的接口。就像插座连接电器一样，套接字连接计算机和应用程序。

2. 在服务器端创建套接字以后，会依次调用 listen 函数和 accept 函数。请比较二者作用。

    调用 listen 函数将套接字转换为可接受连接的状态（监听）。调用 accept 函数受理连接请求，若在没有连接请求的情况下调用该函数，则不会返回，直到有连接请求为止。

3. Linux 中，对套接字数据进行 I/O 时可以直接使用文件 I/O 相关函数；而在 Windows 中则不可以。原因为何？

    Linux把套接字也看作是文件，所以可以用文件I/O相关函数；而Windows要区分套接字和文件，所以设置了特殊的函数。

4. 创建套接字后一般会给他分配地址，为什么？为了完成地址分配需要调用哪个函数？

    套接字创建后，只有为其分配了IP地址和端口号之后，客户端才能通过IP地址和端口号与服务器建立连接；需要调用 bind 函数完成地址分配。

5. Linux 中的文件描述符与 Windows 的句柄实际上非常类似。请以套接字为对象说明它们的含义。

    Linux 的文件描述符
    - 标识资源：文件描述符是一个非负整数，标识每个打开的套接字。
    - 资源操作：通过文件描述符，程序可以对套接字进行读取、写入和关闭等操作。
    
    Windows 的句柄
    - 标识资源：句柄是一个指针或索引值，标识每个创建的套接字。
    - 资源操作：与 Linux 类似

6. 底层 I/O 函数与 ANSI 标准定义的文件 I/O 函数有何区别？

    文件 I/O 又称为低级磁盘 I/O，遵循 POSIX 相关标准。任何兼容 POSIX 标准的操作系统上都支持文件I/O。标准 I/O 被称为高级磁盘 I/O，遵循 ANSI C 相关标准。只要开发环境中有标准 I/O 库，标准 I/O 就可以使用。（Linux 中使用的是 GLIBC，它是标准C库的超集。不仅包含 ANSI C 中定义的函数，还包括 POSIX 标准中定义的函数。因此，Linux 下既可以使用标准 I/O，也可以使用文件 I/O）。

7. 参考本书给出的示例low_open.c和low_read.c，分别利用底层文件 I/O 和 ANSI 标准 I/O 编写文件复制程序。可任意指定复制程序的使用方法。

    暂略。

### 第2章 套接字类型与协议设置

#### 2.1 套接字协议及数据传输特性
##### 2.1.1 创建套接字
```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
/*
成功时返回文件描述符，失败时返回-1
domain: 套接字使用的协议族（Protocol Family）
type: 套接字数据传输的类型信息
protocol: 计算机通信中使用的协议信息
*/
```
##### 2.1.2 协议族（Protocol Family）
协议族表示套接字使用的协议分类信息，可以分为如下几类：
> 头文件 sys/socket.h 中声明的协议族

| 名称 | 协议族 |
| :-: | :-: |
| PF_INET | IPv4 互联网协议族 |
| PF_INET6 | IPv6 互联网协议族 |
| PF_LOCAL | 本地通信 Unix 协议族 |
| PF_PACKET | 底层套接字的协议族 |
| PF_IPX | IPX Novel 协议族 |

本书着重讲 PF_INET 对应的 IPV4 互联网协议族。其他协议并不常用，或并未普及。**另外，套接字中采用的最终的协议信息是通过 socket 函数的第三个参数传递的。在指定的协议族范围内通过第一个参数决定第三个参数。**

##### 2.1.3 套接字类型（Type）
套接字类型指的是套接字的数据传输方式，是通过 socket 函数的第二个参数进行传递的，只有这样才能决定创建的套接字的数据传输方式。**既然已经通过第一个参数传递了协议族信息，为什么还有决定数据传输方式？问题就在于，决定了协议族并不能同时决定数据传输方式。换言之，socket 函数的第一个参数 PF_INET 协议族中也存在多种数据传输方式。**

##### 2.1.4 套接字类型1：面向连接的套接字（SOCK_STREAM）
socket函数的第二个参数为 SOCKET_STREAM，将创建面向连接的套接字。

面向连接的传输方式的特征：
- 传输过程数据不会消失
- 按序传输数据
- 传输的数据不存在数据边界

这种情形对于之前说过的 write 和 read 函数：
> 若传输数据的计算机通过调用3次write 哈四年传递了100字节的数据，但是接收方只需要调用1次read函数就可以接受全部的100个字节

收发数据的套接字内部有缓冲（buffer），简言之就是字节数组。只要不超过数组容量，那么数据填满缓冲后 1 次 read 函数的调用就可以读取全部，同样也有可能调用多次来完成读取。

**套接字缓冲已满是否意味着数据丢失？**
> 答：缓冲并不总是满的。如果读取速度比数据传过来的速度满，则缓冲可能被填满，但这时也不会丢失数据，因为传输套接字此时会停止数据传输，所以面向连接的套接字不会发生数据丢失。

套接字联机必须一一对应。
面向连接的套接字可总结为：
- **可靠地、按序传输的、基于字节的面向连接的数据传输方式的套接字**

##### 2.1.5面向消息的套接字（SOCK_DGRAM）
socket函数的第二个参数为 SOCK_DGRAM 时，将创建面向消息的套接字。
面向消息的套接字可以比喻为 高速移动的摩托车队。 特点如下：
- 强调快速传输而非传输有序
- 传输的数据可能丢失有可能损毁
- 传输的数据有边界
- 限制每次传输数据的大小

相比于面向连接的套接字，面向消息的套接字更具有传输速度，但可能丢失。特点可总结为：
- **不可靠的、不按序传递的、以数据的高速传输为目的的套接字**

##### 2.1.6 协议的最终选择
socket 函数的第3个参数决定最终采用的协议。
前面已经通过前2个参数传递了协议族和套接字数据传输方式，这些信息还不够吗？为什么还要传输第3个参数？
> 可以应对同一协议族中存在多种数据传输方式相同的协议，所以数据传输方式相同，但协议不同，需要用第三个参数指定具体的协议信息。

本书用的是 IPv4 的协议族和面向连接的数据传输，满足这2个条件的协议只有 IPPROTO_TCP，因此可以如下调用 socket 函数创建套接字，这种套接字称为 TCP 套接字。
```c
int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
```
SOCK_DGRAM 指的是面向消息的数据传输方式，满足上述条件的协议只有 TPPROTO_UDP 。这种套接字称为 UDP 套接字：
```c
int udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
```
##### 2.1.7 面向连接的套接字：TCP套接字示例
需要对第一章的代码做出修改，修改好的代码如下：

- [tcp_client.c](https://github.com/wdmhswj/Network_Programming/blob/main/ch2/tcp_client.c)
- [tcp_server.c](https://github.com/wdmhswj/Network_Programming/blob/main/ch2/tcp_server.c)

编译：

```shell
gcc tcp_client.c -o hclient
gcc tcp_server.c -o hserver
```

运行：

```shell
./hserver 9190
./hclient 127.0.0.1 9190
```

结果：

```
Message from server : Hello World! 
Function read call count: 13
```

从运行结果可以看出服务端发送了13字节的数据，客户端调用13次 read 函数进行读取。

#### 2.2 Windows平台下的实现及验证
暂略。
#### 2.3 习题
1. 什么是协议？在收发数据中定义协议有何意义？
    > 答：协议是对话中使用的通信规则，简言之，协议就是为了完成数据交换而定好的约定。在收发数据中定义协议，能够让计算机之间进行对话，以此来实现信息交换和资源共享。
2. 面向连接的套接字 TCP 套接字传输特性有 3 点，请分别说明。
    > 答：1. 传输过程中数据不会消失；2. 按序传输数据；3. 数据不存在数据边界
3. 下面哪些是面向消息的套接字的特点
    - **传输数据可能丢失**
    - 没有数据边界（Boundary）
    - **以快速传递为目标**
    - 不限制每次传输数据大小
    - **与面向连接的套接字不同，不存在连接概念**
4. 下列数据适合用哪类套接字进行传输？
    - 演唱会现场直播的多媒体数据（UDP）
    - 某人压缩过的文本文件（TCP）
    - 网上银行用户与银行之间的数据传递（TCP）
5. 何种类型的套接字不存在数据边界？这类套接字接收数据时应该注意什么？
    > 答：TCP套接字不存在数据边界。在接收数据时，需要保证在接收套接字的缓冲区满时就从buffer中读取数据。也就是，在接收套接字内容，写入buffer的速度要小于读取buffer的速度。

### 第3章 地址族与数据序列
把套接字比喻成电话，那么目前只安装了电话机，本章讲解给电话机分配号码的方法，即给套接字分配IP地址和端口号。

#### 3.1 分配给套接字的 IP 地址与端口号
IP 是 Internet Protocol（网络协议） 的简写，是为收发网络数据而分配给计算机的值。端口号并发赋予计算机的值，而是为了区分程序中创建的套接字而分配给套接字的端口号。

##### 3.1.1 网络地址（Internet Address）
为使计算机连接到网络并收发数据，必须为其分配 IP 地址。IP 地址分为两类。
- IPv4（Internet Protocol version 4）4 字节地址族
- IPv6（Internet Protocol version 6）6 字节地址族

两者之间的主要差别是 IP 地址所用的字节数，目前通用的是 IPV4 , IPV6 的普及还需要时间。

IPV4 标准的 4 字节 IP 地址分为网络地址和主机（指计算机）地址，且分为 A、B、C、D、E 等类型。
![](https://i.loli.net/2019/01/13/5c3ab0eb17bbe.png)
数据传输过程：
![](https://i.loli.net/2019/01/13/5c3ab19174fa4.png)

某主机向 203.211.172.103 和 203.211.217.202 传递数据，其中 203.211.172 和 203.211.217 为该网络的网络地址，所以「向相应网络传输数据」实际上是向构成网络的路由器或者交换机传输数据，然后又路由器或者交换机根据数据中的主机地址向目标主机传递数据。

##### 3.1.2 网络地址分类与主机地址边界
只需通过IP地址的第一个字节即可判断网络地址占用的总字节数，因为我们根据IP地址的边界区分网络地址，如下所示：

- A 类地址的首字节范围为：0~127
- B 类地址的首字节范围为：128~191
- C 类地址的首字节范围为：192~223

还有如下这种表示方式：

- A 类地址的首位以 0 开始
- B 类地址的前2位以 10 开始
- C 类地址的前3位以 110 开始

因此套接字手法数据时，数据传到网络后即可轻松找到主机。

##### 3.1.3 用于区分套接字的端口号
IP地址用于区分计算机，只要有IP地址就能向目标主机传输数据，但是只有这些还不够，我们需要把信息传输给具体的应用程序。

所以计算机一般有 NIC（网络接口卡）数据传输设备。通过 NIC 接受的数据内有端口号，操作系统参考端口号把信息传给相应的应用程序。

端口号由 16 位构成，可分配的端口号范围是 0~65535 。但是 0~1023 是知名端口，一般分配给特定的应用程序，所以应当分配给此范围之外的值。

虽然端口号不能重复，但是 TCP 套接字和 UDP 套接字不会共用端接口号，所以允许重复。如果某 TCP 套接字使用了 9190 端口号，其他 TCP 套接字就无法使用该端口号，但是 UDP 套接字可以使用。

总之，数据传输目标地址同时包含IP地址和端口号，只有这样，数据才会被传输到最终的目的应用程序。

#### 3.2 地址信息的表示
应用程序中使用的IP地址和端口号以结构体的形式给出了定义。本节围绕结构体讨论目标地址的表示方法。
##### 3.2.1 表示 IPv4 地址的结构体
结构体的定义如下
```c
struct sockaddr_in
{
    sa_family_t sin_faminly;    // 地址族（Address Family）
    uint16_t sin_port;  // 16位 TCP/UDP 端口号
    struct in_addr sin_addr;    // 32位 IP 地址
    char sin_zero[8];   // 不使用
};
```
该结构体中提到的另一个结构体 in_addr 定义如下，用来存放 32 位 IP地址：
```c
struct in_addr
{
    in_addr_t s_addr;   // 32位IPv4地址
};
```
关于以上两个结构体的一些数据类型。

| 数据类型名称 |             数据类型说明             | 声明的头文件 |
| :----------: | :----------------------------------: | :----------: |
|   int 8_t    |           signed 8-bit int           | sys/types.h  |
|   uint8_t    |  unsigned 8-bit int (unsigned char)  | sys/types.h  |
|   int16_t    |          signed 16-bit int           | sys/types.h  |
|   uint16_t   | unsigned 16-bit int (unsigned short) | sys/types.h  |
|   int32_t    |          signed 32-bit int           | sys/types.h  |
|   uint32_t   | unsigned 32-bit int (unsigned long)  | sys/types.h  |
| sa_family_t  |       地址族（address family）       | sys/socket.h |
|  socklen_t   |       长度（length of struct）       | sys/socket.h |
|  in_addr_t   |       IP地址，声明为 uint_32_t       | netinet/in.h |
|  in_port_t   |       端口号，声明为 uint_16_t       | netinet/in.h |

为什么要额外定义这些数据类型呢？这是考虑扩展性的结果

##### 3.2.2 结构体 sockaddr_in 的成员分析
- 成员 sin_family
每个协议族适用的地址族不同，比如，IPv4使用 4 字节的地址族，IPv6使用16字节的地址族。
> 地址族

| 地址族（Address Family）| 含义 |
| :-: | :-: |
| AF_INET | IPv4用的地址族|
| AF_INET6 | IPv6 用的地址族 |
| AF_LOCAL | 本地通信中采用的Unix协议的地址族 |

AF_LOACL 只是为了说明具有多种地址族而添加的。

- 成员 sin_port

    该成员保存 16 位端口号，重点在于，它以网络字节序保存。

- 成员 sin_addr

    该成员保存 32 为IP地址信息，且也以网络字节序保存

- 成员 sin_zero

    无特殊含义。只是为结构体 sockaddr_in 的大小与sockaddr结构体保持一致而插入的成员。填充零。

在之前的代码中 sockaddr_in 结构体变量地址值将以如下方式传递给 bind 函数。
```c
if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error");
```
此处 bind 的第二个参数期望得到的是 sockaddr 结构体变量的地址值，包括地址族、端口号、IP地址等。
```c
struct sockaddr
{
    sa_family_t sin_family; // 地址族
    char sa_data[14];   // 地址信息
};
```
sockaddr_in结构体作用：由于直接让结构体sockaddr包含所需信息比较麻烦，所以用sockaddr_in结构体强制转换

此结构体中 sa_data 保存的地址信息中需要包含IP地址和端口号，剩余部分应该填充0，但是这样对于包含地址的信息非常麻烦，所以出现了sockaddr_in结构体，然后强制转换成sockaddr类型，则生成符合bind条件的参数。

#### 3.3 网络字节序与地址变换
不同的 CPU 中，4 字节整数值1在内存空间保存方式是不同的。

有些 CPU 这样保存：
```
00000000 00000000 00000000 00000001
```
有些 CPU 这样保存：
```
00000001 00000000 00000000 00000000
```
两种一种是顺序保存，一种是倒序保存 。

##### 3.3.1 字节序（Order）与网络字节序
CPU保存数据的方式有2种，这意味着CPU解析数据的方式也有2种：
- 大端序（Big Endian）：高字节存放到低地址
- 小端序（Little Endian）：低位字节存放到低位地址
![big.png](https://i.loli.net/2019/01/13/5c3ac9c1b2550.png)
![small.png](https://i.loli.net/2019/01/13/5c3ac9c1c3348.png)
两台字节序不同的计算机在数据传递的过程种可能出现的问题：
![zijiexu.png](https://i.loli.net/2019/01/13/5c3aca956c8e9.png)
因为这种原因，所以在通过网络传输数据时必须约定统一的方式，这种约定被称为网络字节序，非常简单，统一为大端序。即，先把数据数组转化成大端序格式再进行网络传输。

##### 3.3.2 字节序转换
帮助转换字节序的函数：
```c
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long);
unsigned long ntohl(unsigned long);
```
通过函数名称掌握其功能，只需要了解：
- htons 的 h 代表主机（host）字节序。
- htons 的 n 代表网络（network）字节序。
- s 代表 short
- l 代表 long

下面的代码是示例，说明以上函数调用过程：

```c
#include <stdio.h>
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port); //转换为网络字节序
    net_addr = htonl(host_addr);

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered address: %#lx \n", host_addr);
    printf("Network ordered address: %#lx \n", net_addr);

    return 0;
}
```
编译运行：

```shell
gcc endian_conv.c -o conv
./conv
```

结果：

```
Host ordered port: 0x1234
Network ordered port: 0x3412
Host ordered address: 0x12345678
Network ordered address: 0x78563412
```

这是在小端 CPU 的运行结果。大部分人会得到相同的结果，因为 Intel 和 AMD 的 CPU 都是小端序为标准。

#### 3.4 网络地址的初始化与分配
##### 3.4.1 将字符串信息转换为网络字节序的整数型
sockaddr_in 中需要的是32位整数型，但是我们只熟悉点分十进制表示法，那么改如何把类似于201.211.214.36转换为4字节的整数类型数据呢？幸运的是，有一个函数可以帮助我们完成它。
```c
#include <arpa/inet.h>
in_addr_t inet_addr(const char* string);
```
具体示例：
```c
#include <stdio.h>
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    return 0;
}
```
编译运行：
```
gcc inet_addr.c -o addr
./addr
```
输出：
```
Network ordered integer addr: 0x4030201
Error occured!
```
1个字节能表示的最大整数是255，所以代码中 addr2 是错误的IP地址。从运行结果看，inet_addr 不仅可以转换地址，还可以检测有效性。

inet_aton 函数与 inet_addr 函数在功能上完全相同，也是将字符串形式的IP地址转换成整数型的IP地址。只不过该函数用了 in_addr 结构体，且使用频率更高。

```c
#include <arpa/inet.h>
int inet_aton(const char *string, struct in_addr *addr);
/*
成功时返回 1 ，失败时返回 0
string: 含有需要转换的IP地址信息的字符串地址值
addr: 将保存转换结果的 in_addr 结构体变量的地址值
*/
```
函数调用示例：
```c
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if (!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("Conversion error");
    else
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

编译运行：
```
gcc inet_aton.c -o aton
./aton
```
运行结果：
```
Network ordered integer addr: 0x4f7ce87f
```
可以看出，已经成功的把转换后的地址放进了 addr_inet.sin_addr.s_addr 中。

还有一个函数，与 inet_aton() 正好相反，它可以把网络字节序整数型IP地址转换成我们熟悉的字符串形式，函数原型如下：
```c
#include <arpa/inet.h>
char *inet_ntoa(struct in_addr adr);
```
该函数将通过参数传入的整数型IP地址转换为字符串格式并返回。但要小心，返回值为 char 指针，返回字符串地址意味着字符串已经保存在内存空间，但是该函数未向程序员要求分配内存，而是再内部申请了内存保存了字符串。也就是说调用了该函数候要立即把信息复制到其他内存空间。因此，若再次调用 inet_ntoa 函数，则有可能覆盖之前保存的字符串信息。总之，再次调用 inet_ntoa 函数前返回的字符串地址是有效的。若需要长期保存，则应该将字符串复制到其他内存空间。

示例：
```c
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);
    //把addr1中的结构体信息转换为字符串的IP地址形式
    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation1: %s \n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s \n", str_ptr);
    printf("Dotted-Decimal notation3: %s \n", str_arr);
    return 0;
}
```

编译运行：

```
gcc inet_ntoa.c -o ntoa
./ntoa
```
输出:
```
Dotted-Decimal notation1: 1.2.3.4
Dotted-Decimal notation2: 1.1.1.1
Dotted-Decimal notation3: 1.2.3.4
```
##### 3.4.2 网络地址初始化
结合前面的内容，介绍套接字创建过程中，常见的网络信息初始化方法：
```c
struct sockaddr_in addr;
char* serv_ip = "211.217.168.13";   // 声明IP地址族
char* serv_port = "9190";   // 声明端口号字符串
memset(&addr, 0, sizeof(addr)); // 结构体变量 addr 的所有成员都初始化为0
addr.sin_family = AF_INET;  // 制定地址族
addr.sin_addr.s_addr = inet_addr(serv_ip);  // 基于字符串的IP地址初始化
addr.sin_port = htons(atoi(serv_port)); // 基于字符串的IP地址端口号初始化
```
#### 3.5 基于Windows的实现
暂略。
#### 3.6 习题
1. IP地址族 IPV4 与 IPV6 有什么区别？在何种背景下诞生了 IPV6?
    > 答：主要差别是IP地址所用的字节数，目前通用的是IPV4，目前IPV4的资源已耗尽，所以诞生了IPV6，它具有更大的地址空间。

2. 通过 IPV4 网络 ID 、主机 ID 及路由器的关系说明公司局域网的计算机传输数据的过程
    > 答：网络ID是为了区分网络而设置的一部分IP地址，假设向www.baidu.com公司传输数据，该公司内部构建了局域网。因为首先要向baidu.com传输数据，也就是说并非一开始就浏览所有四字节IP地址，首先找到网络地址，进而由baidu.com（构成网络的路由器）接收到数据后，传输到主机地址。比如向 203.211.712.103 传输数据，那就先找到 203.211.172 然后由这个网络的网关找主机号为 172 的机器传输数据。

3. 套接字地址分为IP地址和端口号，为什么需要IP地址和端口号？或者说，通过IP地址可以区分哪些对象？通过端口号可以区分哪些对象？
    > 答：有了IP地址和端口号，才能把数据准确的传送到某个应用程序中。通过IP地址可以区分具体的主机，通过端口号可以区分主机上的应用程序。

4. 请说明IP地址的分类方法，并据此说出下面这些IP的分类。
    - 214.121.212.102（C类）
    - 120.101.122.89（A类）
    - 129.78.102.211（B类）
    
    分类方法：A 类地址的首字节范围为：0~127、B 类地址的首字节范围为：128~191、C 类地址的首字节范围为：192~223

5. 计算机通过路由器和交换机连接到互联网，请说出路由器和交换机的作用。
    > 答：路由器和交换机完成外网和本网主机之间的数据交换。

6. 什么是知名端口？其范围是多少？知名端口中具有代表性的 HTTP 和 FTP 的端口号各是多少？
    > 答：知名端口是要把该端口分配给特定的应用程序，范围是 0~1023 ，HTTP 的端口号是 80 ，FTP 的端口号是20和21

7. **向套接字分配地址的 bind 函数原型如下：**

   ```c
   int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
   ```

   **而调用时则用：**

   ```c
   bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)
   ```

   **此处 serv_addr 为 sockaddr_in 结构体变量。与函数原型不同，传入的是 sockaddr_in 结构体变量，请说明原因。**
    > 答：因为对于详细的地址信息使用 sockaddr 类型传递特别麻烦，进而有了 sockaddr_in 类型，其中基本与前面的类型保持一致，还有 sa_sata[4] 来保存地址信息，剩余全部填 0，所以强制转换后，不影响程序运行。

8. 请解释大端序，小端序、网络字节序，并说明为何需要网络字节序。
    > 答：CPU 向内存保存数据有两种方式，大端序是高位字节存放低位地址，小端序是高位字节存放高位地址，网络字节序是为了方便传输的信息统一性，统一成了大端序。

9. 大端序计算机希望把 4 字节整数型 12 传递到小端序计算机。请说出数据传输过程中发生的字节序变换过程。
    > 答：0x0000000c->0x0000000c->0x0c000000

10. 怎样表示回送地址？其含义是什么？如果向回送地址处传输数据将会发生什么情况？
    > 答：127.0.0.1 表示回送地址，指的是计算机自身的IP地址，无论什么程序，一旦使用回送地址发送数据，协议软件立即返回，不进行任何网络传输。
### 第4章 基于TCP的服务器端/客户端（1）
#### 4.1 理解 TCP 和 UDP
TCP套接字是面向连接的，又称为基于流的套接字。TCP的全称为 Transmission Control Protocol（传输控制协议）。

##### 4.1.1 TCP/IP 协议栈
![](https://i.loli.net/2019/01/14/5c3c21889db06.png)
TCP/IP 协议栈共分为 4 层，可以理解为数据收发分成了 4 个层次化过程，通过层次化的方式来解决问题。

**链路层**
链路层是物理链接领域标准化的结果，也是最基本的领域，专门定义 LAN、WAN、MAN等网络标准。若两台主机通过网络进行数据交换，则需要物理连接，链路层就负责这些标准。

**IP 层**
准备好物理连接后就要进行传输数据，首先需要考虑路径的选择。
IP本身是面向消息、不可靠的协议，无法应对数据错误。

**TCP/UDP**
IP 层解决数据传输中的路径选择问题，只需照此路径传输数据即可。TCP 和 UDP 层以 IP 层提供的路径信息为基础完成实际的数据传输，故该层又称为传输层。UDP 比 TCP 简单，现在我们只解释 TCP 。 TCP 可以保证数据的可靠传输，但是它发送数据时以 IP 层为基础（这也是协议栈层次化的原因）

IP 层只关注一个数据包（数据传输基本单位）的传输过程。因此，即使传输多个数据包，每个数据包也是由 IP 层实际传输的，也就是说传输顺序及传输本身是不可靠的。若只利用IP层传输数据，则可能导致后传输的数据包B比先传输的数据包A提早到达。另外，传输的数据包A、B、C中可能只收到A和C，甚至收到的C可能已经损毁 。反之，若添加 TCP 协议则按照如下对话方式进行数据交换。

> 主机A：正确接受第二个数据包
>
> 主机B：恩，知道了
>
> 主机A：正确收到第三个数据包
>
>主机B：可我已经发送第四个数据包了啊！哦，您没收到吧，我给你重新发。

这就是 TCP 的作用。如果交换数据的过程中可以确认对方已经收到数据，并重传丢失的数据，那么即便IP层不保证数据传输，这类通信也是可靠的。
![](https://i.loli.net/2019/01/14/5c3c268b40be6.png)
**应用层**
上述内容是套接字通信过程中自动处理的。选择数据传输路径、数据确认过程都被隐藏到套接字内部。向程序员提供的工具就是套接字，只需要利用套接字编出程序即可。编写软件的过程中，需要根据程序的特点来决定服务器和客户端之间的数据传输规则，这便是应用层协议。




### 第10章 多进程服务器端
#### 创建进程
通过调用`fork`函数创建进程
```cpp
#include <unistd.h>
pid_t fork(void);   // 成功时返回进程ID，失败时返回-1
```
fork函数的作用：复制正在运行的、调用fork函数的父进程，生成子进程；两个进程都将执行fork函数返回后的语句，同时具有完全独立的内存结构。

