#ifndef BUNDLE_H
#define BUNDLE_H

#include <string>
#include <sys/epoll.h>
#include <iostream>

#include "../parser.h"
#include "Db.h"
#include "MatrixServer.h"

#define CONFIG_THREADS			"threadspool"
#define CONFIG_DB						"db"
#define CONFIG_PORT					"port"
#define CONFIG_THREADS_DEF	4
#define CONFIG_DB_DEF				"qm.db"
#define CONFIG_PORT_DEF			"53490"

class Bundle {
private:
	MatrixServer* a_matrix;
	Db* a_db;
	int a_epfd;
	int a_threadNo; //for init only, not used in the threads
	std::string a_portNo; //for init only, not used in the threads

public:
	Bundle();
	~Bundle();

	MatrixServer* matrix() {return a_matrix;}
	Db* db() {return a_db;}
	std::string portNo() const {return a_portNo;}
	int threadsNo() const {return a_threadNo;}
	int epfd() {return a_epfd;}
};

#endif
