#ifndef MAIN_H
#define MAIN_H

#include <fstream>
#include <vector>
#include <pthread.h>
#include <unistd.h>

#include "connection.h"
#include "Bundle.h"

void threadsLaunch(Bundle* serverData);
void* threadStart(void *arg); //from threads.cpp

#endif
