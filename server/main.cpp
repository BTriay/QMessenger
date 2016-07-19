#include "main.h"

int main() {
	Bundle serverData;
	bundleInit(serverData);
	threadsLaunch(serverData);
	int sockfd = serverInit(serverData.portNo.c_str());
	s_accept(sockfd, serverData.epfd);
	bundleClean(serverData); //never reaches this point
	return 0;
}

void bundleInit(Bundle& serverData) {
	std::vector<std::string> configTokens;
	std::vector<std::string>::iterator it;
	if (!configFileReader("config", configTokens))
		std::cout << "no config file, error\n";

	for (it = configTokens.begin(); it != configTokens.end(); it++) {
		if (*it == CONFIG_DB) {
			it++;
			std::ifstream f((*it).c_str(), std::ios_base::in);
			serverData.db = new Db(*it, !f.good());
		}
		else if (*it == CONFIG_THREADS) {
			it++;
			serverData.threadNo = stoi(*it);
		}
		else if (*it == CONFIG_PORT) {
			it++;
			serverData.portNo = *it;
		}
	}
	serverData.matrix = new MatrixServer();
	serverData.epfd = epoll_create1(0);
	if (serverData.epfd == -1) {
		std::cout << "error creating epoll\n";
		return;
	}
}

void bundleClean(Bundle& serverData) {
	delete serverData.db;
	delete serverData.matrix;
}

void threadsLaunch(Bundle& serverData) {
	int s;
	std::vector<pthread_t> threads(serverData.threadNo);
	for (int i = 0; i < serverData.threadNo; i++) {
		s = pthread_create(&threads[i], NULL, threadStart, (void *) (&serverData));
		if (s != 0)
			std::cout << "error thread creation\n";
	}
}
