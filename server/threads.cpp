#include "main.h"

#define BUF_SZ		256

static pthread_mutex_t mtxEpoll = PTHREAD_MUTEX_INITIALIZER;

void* threadStart(void *arg) {

	pthread_detach(pthread_self());
	Bundle *serverData = reinterpret_cast<Bundle *> (arg);
	Db* db = serverData->db;
	MatrixServer* matrix = serverData->matrix;

	std::string msg;
	std::vector<std::string> msgTokens;

	struct epoll_event ev[1];
	int fd;
	int event;
	int i;

	while (true) {
		if (pthread_mutex_lock(&mtxEpoll) != 0)
			std::cout << "mutex lock pb\n";

		if (epoll_wait(serverData->epfd, ev, 1, -1) == -1)
			std::cout << "error epoll_wait\n";
		event = ev[0].events;
		fd = ev[0].data.fd;

		if (pthread_mutex_unlock(&mtxEpoll) != 0)
			std::cout << "mutex unlock pb\n";

		if (event & EPOLLRDHUP) {
//			matrix->rmUser(ev[0].data.fd);
			close(ev[0].data.fd);
			std::cout << "peer hangup\n";
		}
		else if (event & EPOLLIN) {
			msg.clear();
			getSocketMsg(msg, fd);
			if (!msg.empty()) //{
				std::cout << "msg: " << msg << std::endl << std::endl;
/*
				msgTokens.clear();
				i = msgReader(msg, msgTokens);
				switch (i) {
					case ROOM_MSG:
						break;
					case KEEPALIVE:
						break;
	//... many more cases to handle
				}
			}
*/
			rearmFD(serverData->epfd, fd);
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
	msgSz = ntohs(static_cast<uint16_t> (len));//..if problem here

	while (msgSz) {
		i = read(fd, c_msg, msgSz >= BUF_SZ ? BUF_SZ -1 : msgSz);
		if (i == -1) { //..will read as much as possible here and discard
			std::cout << "error read msg\n";
			msg.clear();
			return;
		}
		c_msg[i] = '\0';
		msg += c_msg;
		msgSz -= i;
	}
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
