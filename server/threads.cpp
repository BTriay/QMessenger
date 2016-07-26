#include "ThreadFunc.h"

#define BUF_SZ		256

static pthread_mutex_t mtxEpoll = PTHREAD_MUTEX_INITIALIZER;

void* threadStart(void *arg) {

	pthread_detach(pthread_self());
	ThreadFunc func(reinterpret_cast<Bundle *> (arg));

	std::string msg;
	std::vector<std::string> msgTokens;

	struct epoll_event ev[1];
	int fd;
	int event;
	int i;

	while (true) {
		if (pthread_mutex_lock(&mtxEpoll) != 0)
			std::cout << "mutex lock pb\n";

		if (epoll_wait(func.epfd(), ev, 1, -1) == -1)
			std::cout << "error epoll_wait\n";
		event = ev[0].events;
		fd = ev[0].data.fd;

		if (pthread_mutex_unlock(&mtxEpoll) != 0)
			std::cout << "mutex unlock pb\n";

		if (event & EPOLLRDHUP) {
			func(RM_USER, fd, msgTokens);
			close(fd);
			std::cout << "peer hangup\n";
		}
		else if (event & EPOLLIN) {
			msg.clear();
			getSocketMsg(msg, fd);
			if (!msg.empty()) {
				msgTokens.clear();
				i = msgParser(msg, msgTokens);
				if (i == -1)
					emptySocket(fd);
				else
					func(i, fd, msgTokens);
			}
			rearmFD(func.epfd(), fd);
		}
	}
}

void getSocketMsg(std::string& msg, int fd) {
	char c_msg[BUF_SZ];
	uint16_t len;
	int msgSz;
	int i = 0;

	i = read(fd, &len, sizeof(uint16_t));
	if (i == -1) {
		std::cout << "error read msg\n"; //if errno = EAGAIN then ignore and return msg="";
		return;
	}
	msgSz = ntohs(static_cast<uint16_t> (len));
	//if problem with msgSz, then
	//	emptySocket(fd);

	while (msgSz) {
		i = read(fd, c_msg, msgSz >= BUF_SZ ? BUF_SZ -1 : msgSz);
		if (i == -1) {
			std::cout << "error read msg\n";
			msg.clear();
			return;
		}
		c_msg[i] = '\0';
		msg += c_msg;
		msgSz -= i;
	}
}

void emptySocket(int fd) {
//read this socket until it returns 0. Potentially several messages lost
	char c_msg[BUF_SZ];
	int i = 1;
	while (i)
		i = read(fd, c_msg, BUF_SZ);
}

void rearmFD(int epfd, int fd) {
	int s;
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT;
	ev.data.fd = fd;
	s = epoll_ctl(epfd, EPOLL_CTL_MOD , fd, &ev);
	if (s < 0)
		std::cout << "error: epoll_ctl\n";
}
