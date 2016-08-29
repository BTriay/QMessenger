#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#ifdef TEST_COUT
#include <iostream>
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux
#include <arpa/inet.h>
#elif _WIN32
#include <winsock2.h>
#endif

#include "parserConst.h"

#define BUF_SZ			256

#ifdef CLIENT
    #include "Socket.h"
    class Socket;
#endif


class MsgWriter {
private:
	char * a_buf;
	size_t a_sz;
public:
    MsgWriter(const std::vector<std::string>& tokens, int identifier);
    MsgWriter(const std::string& token, int identifier);
    explicit MsgWriter(int identifier);
	~MsgWriter();
	char* getMsg() const { return a_buf; }
	size_t getSz() const { return a_sz; }
};

bool configFileReader(const std::string& filename, std::vector<std::string>& tokens);
int msgParser(const std::string& msg, std::vector<std::string>& tokens);
void emptySocket(int socket);
#ifdef SERVER
void getSocketMsg(std::string& msg, int fd);
#endif
#ifdef CLIENT
void getSocketMsg(std::string& msg, Socket* a_socket);
#endif

#endif
