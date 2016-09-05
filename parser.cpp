#include "parser.h"

MsgWriter::MsgWriter(const std::vector<std::string>& tokens, int identifier) {
	std::string msg = std::to_string(identifier) + "\n";
  for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
		msg += *it + "\n";

  uint16_t sz = msg.length();
  uint16_t nsz = htons(sz);

  a_sz = sz + sizeof(uint16_t);
  a_buf = reinterpret_cast<char*> (malloc(a_sz));

  memcpy(a_buf, &nsz, sizeof(uint16_t));
  memcpy(a_buf + sizeof(uint16_t), msg.c_str(), sz);
}

MsgWriter::MsgWriter(const std::string& token, int identifier) {
  std::string msg = std::to_string(identifier) + "\n" + token + "\n";

  uint16_t sz = msg.length();
  uint16_t nsz = htons(sz);

  a_sz = sz + sizeof(uint16_t);
  a_buf = reinterpret_cast<char*> (malloc(a_sz));

  memcpy(a_buf, &nsz, sizeof(uint16_t));
  memcpy(a_buf + sizeof(uint16_t), msg.c_str(), sz);
}

MsgWriter::MsgWriter(int identifier) {
  std::string msg = std::to_string(identifier) + "\n";

  uint16_t sz = msg.length();
  uint16_t nsz = htons(sz);

  a_sz = sz + sizeof(uint16_t);
  a_buf = reinterpret_cast<char*> (malloc(a_sz));

  memcpy(a_buf, &nsz, sizeof(uint16_t));
  memcpy(a_buf + sizeof(uint16_t), msg.c_str(), sz);
}

MsgWriter::~MsgWriter() {
	free(a_buf);
}

bool configFileReader(const std::string& filename, std::vector<std::string>& tokens) {
	int i;
	std::string line;
	std::ifstream file(filename.c_str(), std::ios::in);
	if (file) {
		while (file >> line) {
			i = line.find("=", 0);
			tokens.push_back(line.substr(0,i));
			tokens.push_back(line.substr(i+1,std::string::npos-1));
			file.clear();
		}
		file.close();
		return true;
	}
	else
		return false;
}

int msgParser(const std::string& msg, std::vector<std::string>& tokens) {
	std::stringstream s(msg, std::ios_base::in);
	int i;
	s >> i;
	s.ignore();

	std::string str;
	while (getline(s, str))
		tokens.push_back(str);
	return i;
}

#ifdef SERVER
void getSocketMsg(std::string& msg, int fd) {
#endif
#ifdef CLIENT
void getSocketMsg(std::string& msg, Socket* a_socket) {
#endif
    char c_msg[BUF_SZ];
    uint16_t len;
    size_t msgSz;
    int i = 0;

#ifdef SERVER
    read(fd, &len, sizeof(uint16_t));
#endif
#ifdef CLIENT
    if (!a_socket->getMsgSz(&len))
        return;
#endif

    msgSz = ntohs(static_cast<uint16_t> (len));

    while (msgSz) {
#ifdef SERVER
        i = read(fd, c_msg, msgSz >= BUF_SZ ? BUF_SZ -1 : msgSz);
#endif
#ifdef CLIENT
        i = a_socket->getMsg(c_msg, msgSz >= BUF_SZ ? BUF_SZ -1 : msgSz);
#endif
        if (i == -1) {
            msg.clear();
            return;
        }
        c_msg[i] = '\0';
        msg += c_msg;
        msgSz -= i;
    }
}

#ifdef SERVER
//read this socket until it returns 0. Potentially several messages lost
void emptySocket(int fd) {
    char c_msg[BUF_SZ];
    int i = 1;
    while (i <= UINT16_MAX)
        i = read(fd, c_msg, BUF_SZ);
}
#endif
