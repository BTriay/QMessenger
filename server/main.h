#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <sys/epoll.h>

#include "../parser.h"
#include "connection.h"
#include "Db.h"
#include "MatrixServer.h"

typedef struct Bundle Bundle;

struct Bundle {
	MatrixServer* matrix;
	Db* db;
	int epfd; //for init only, not used in the threads
	int threadNo; //for init only, not used in the threads
	std::string portNo; //for init only, not used in the threads
};

void bundleInit(Bundle& mydata);
void bundleClean(Bundle& serverData);
void threadsLaunch(Bundle& serverData);
void* threadStart(void *arg);

void getSocketMsg(std::string& msg, int fd);
void rearmFD(int epfd, int fd);
