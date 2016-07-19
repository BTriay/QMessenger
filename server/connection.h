#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include <iostream>

#define BACKLOG				10

int serverInit(const char* port); //returns the listening socket fd
int ssocket();
int sbind(const char* port);
void getaddrinfoRes(struct addrinfo **res, const char* port);
void slisten(int sock);
void s_accept(int sockfd, int epfd);
void epollList(int sock, int epfd);
