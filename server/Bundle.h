#ifndef BUNDLE_H
#define BUNDLE_H

typedef struct Bundle Bundle;

struct Bundle {
	MatrixServer* matrix;
	Db* db;
	int epfd;
	int threadNo; //for init only, not used in the threads
	std::string portNo; //for init only, not used in the threads
};

#endif
