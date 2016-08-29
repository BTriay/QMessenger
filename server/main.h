#ifndef MAIN_H
#define MAIN_H

#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "../parser.h"
#include "connection.h"
#include "Db.h"
#include "MatrixServer.h"
#include "Bundle.h"

#define CONFIG_THREADS			"threadspool"
#define CONFIG_DB						"db"
#define CONFIG_PORT					"port"
#define CONFIG_THREADS_DEF	4
#define CONFIG_DB_DEF				"qm.db"
#define CONFIG_PORT_DEF			"53490"

void bundleInit(Bundle& mydata);
void bundleClean(Bundle& serverData);
void threadsLaunch(Bundle& serverData);
void* threadStart(void *arg);

#endif
