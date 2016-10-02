#include "ThreadFunc.h"

static pthread_mutex_t mtxEpoll = PTHREAD_MUTEX_INITIALIZER;

void* threadStart(void *arg) {
	pthread_detach(pthread_self());
	ThreadFunc func(reinterpret_cast<Bundle *> (arg));

	std::vector<std::string> msgTokens;
	struct epoll_event ev[1];
	int fd;
	int event;

	while (true) {
		{
			Locker l(&mtxEpoll);
			epoll_wait(func.epfd(), ev, 1, -1);
			event = ev[0].events;
			fd = ev[0].data.fd;
		}

		if (event & EPOLLRDHUP)
			func(USER_HANGUP, fd, msgTokens);
		else if (event & EPOLLIN) {
			std::string msg;
			getSocketMsg(msg, fd);
			if (!msg.empty()) {
				#ifdef TEST_COUT
				std::cout << "\n**msg start**\n" << msg << "**msg end**\n";
				#endif
				msgTokens.clear();
				int i = msgParser(msg, msgTokens);
				func(i, fd, msgTokens);
			}
			rearmFD(func.epfd(), fd);
		}
	}
}

void rearmFD(int epfd, int fd) {
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT;
	ev.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_MOD , fd, &ev);
}
