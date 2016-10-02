#include "main.h"

int main() {
//	daemon(1,0);
	Bundle serverData;
	threadsLaunch(&serverData);
	int sockfd = serverInit(serverData.portNo().c_str());
	s_accept(sockfd, serverData.epfd());
	return 0;
}

void threadsLaunch(Bundle* serverData) {
	std::vector<pthread_t> threads(serverData->threadsNo());
	for (int i = 0; i < serverData->threadsNo(); i++)
		pthread_create(&threads[i], NULL, threadStart, (void *) (serverData));
}
