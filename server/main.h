#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <sys/epoll.h>

#include "../parser.h"
#include "connection.h"
#include "Db.h"
#include "MatrixServer.h"
#include "Bundle.h"

//main.cpp
void bundleInit(Bundle& mydata);
void bundleClean(Bundle& serverData);
void threadsLaunch(Bundle& serverData);
void* threadStart(void *arg);
