#include "connection.h"

int serverInit(const char* port) {
	int sock = sbind(port);
	slisten(sock);
	return sock;
}

int ssocket() {
	int sock;
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		std::cout << "error: socket\n";
	return sock;
}

int sbind(const char* port) {
	struct addrinfo *res, *p;
	getaddrinfoRes(&res, port);

	int sock;
	for (p=res; p != NULL ; p = p->ai_next ) {
		sock = ssocket();
		if ( bind(sock, p->ai_addr, p->ai_addrlen) < 0) {
			close(sock);
			continue;
		}
		break;
	}
	if (p == NULL)
		std::cout << "error: bind\n";
	return sock;
}

void getaddrinfoRes(struct addrinfo **res, const char* port) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, res) < 0)
		std::cout << "error: getaddrinfoRes\n";
}

void slisten(int sock) {
	if (listen(sock, BACKLOG) < 0)
		std::cout << "error: listen\n";
}

void s_accept(int sockfd, int epfd) {
	int new_sockfd;
	int flags;
std::cout << "waiting for new connections\n";
/*
	while (true) {
		if ( new_sockfd = accept(sockfd, NULL, NULL) < 0)
			std::cout << "error: accept\n";
		else if (new_sockfd > 0) {
			flags = fcntl(new_sockfd, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(new_sockfd, F_SETFL, flags);
			epollList(new_sockfd, epfd);
		}
	}
*/
}

void epollList(int sock, int epfd) {
	int s;
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT;
	ev.data.fd = sock;

	s = epoll_ctl(epfd, EPOLL_CTL_ADD , sock, &ev);
	if (s < 0)
		std::cout << "error: epoll_ctl\n";
}
