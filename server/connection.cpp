#include "connection.h"

int serverInit(const char* port) {
	int sock = sbind(port);
	int i = slisten(sock);
	if (i < 0)
		return -1;
	return sock;
}

int ssocket() {
	int sock;
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
#ifdef TEST_COUT
		std::cout << "error: socket\n";
#endif
		return -1;
	}
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
	if (p == NULL) {
#ifdef TEST_COUT
		std::cout << "error: bind\n";
#endif
		return -1;
	}
	return sock;
}

int getaddrinfoRes(struct addrinfo **res, const char* port) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, res) < 0) {
#ifdef TEST_COUT
		std::cout << "error: getaddrinfoRes\n";
#endif
		return -1;
	}
	return 0;
}

int slisten(int sock) {
	if (listen(sock, BACKLOG) < 0) {
//		std::cout << "error: listen\n";
		return -1;
	}
	return 0;
}

void s_accept(int sockfd, int epfd) {
	int new_sockfd;
	int flags;
	int s;
	while (true) {
		if ( (new_sockfd = accept(sockfd, NULL, NULL)) < 0)
#ifdef TEST_COUT
			std::cout << "error: accept. Error: " << errno << std::endl;
#endif
			;
		else
			s = flags = fcntl(new_sockfd, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(new_sockfd, F_SETFL, flags);
			epollList(new_sockfd, epfd);
	}
}

void epollList(int sock, int epfd) {
	int s;
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT;
	ev.data.fd = sock;

	s = epoll_ctl(epfd, EPOLL_CTL_ADD , sock, &ev);
#ifdef TEST_COUT
	if (s < 0)
		std::cout << "error: epoll_ctl\n";
#endif
}
