#include "main.h"

int main() {
	daemon(1,0);
	Bundle serverData;
	bundleInit(serverData);
	threadsLaunch(serverData);
	int sockfd = serverInit(serverData.portNo.c_str());
	s_accept(sockfd, serverData.epfd);
	bundleClean(serverData); //never reaches this point
	return 0;
}

void bundleInit(Bundle& serverData) {
	bool conf_threads = false;
	bool conf_port = false;
	bool conf_db = false;

	std::vector<std::string> configTokens;
	if (configFileReader("config", configTokens)) {
		for (std::vector<std::string>::iterator it = configTokens.begin(); it != configTokens.end(); it++) {
			if (*it == CONFIG_DB) {
				it++;
				std::ifstream f((*it).c_str(), std::ios_base::in);
				serverData.db = new Db(*it, !f.good());
				conf_db = true;
			}
			else if (*it == CONFIG_THREADS) {
				it++;
				serverData.threadNo = stoi(*it);
				conf_threads = true;
			}
			else if (*it == CONFIG_PORT) {
				it++;
				serverData.portNo = *it;
				conf_port = true;
			}
		}
	}
//if config file does not exist or is missing some info, use default configuration
	if (!conf_threads)
		serverData.threadNo = CONFIG_THREADS_DEF;
	if (!conf_port)
		serverData.portNo = CONFIG_PORT_DEF;
	if (!conf_db) {
		std::ifstream f(CONFIG_DB_DEF, std::ios_base::in);
		serverData.db = new Db(CONFIG_DB_DEF, !f.good());
	}

	serverData.matrix = new MatrixServer();
	serverData.epfd = epoll_create1(0);
}

void bundleClean(Bundle& serverData) {
	delete serverData.db;
	delete serverData.matrix;
}

void threadsLaunch(Bundle& serverData) {
	std::vector<pthread_t> threads(serverData.threadNo);
	for (int i = 0; i < serverData.threadNo; i++)
		pthread_create(&threads[i], NULL, threadStart, (void *) (&serverData));
}
