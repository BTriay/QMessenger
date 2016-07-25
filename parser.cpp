#include "parser.h"

bool configFileReader(std::string const & filename, std::vector<std::string>& tokens) {
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

int msgReader(std::string const & msg, std::vector<std::string>& tokens) {
	std::stringstream s(msg, std::ios_base::in);
	int i;
	s >> i;
	s.ignore();
	std::string str;
	while (getline(s, str))
		tokens.push_back(str);
	return i;
}

char* msgWriter(std::string& msg, std::vector<std::string>& tokens, int identifier, size_t* size) {
	std::vector<std::string>::iterator it;
	msg += std::to_string(identifier) + "\n";
	for (it = tokens.begin(); it != tokens.end(); it++)
		msg+= *it + "\n";

    uint16_t sz = msg.length();
    uint16_t nsz = htons(sz);
    *size = sz + sizeof(uint16_t);
    char* buf = reinterpret_cast<char*> (malloc(sz + sizeof(uint16_t) + (size_t)1));
    memcpy(buf, &nsz, sizeof(uint16_t));
    memcpy(buf+sizeof(uint16_t), msg.c_str(), sz + sizeof(uint16_t));
    buf[sz + sizeof(uint16_t) + 1] = '\0';
	return buf;	
}
