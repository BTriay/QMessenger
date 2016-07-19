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

void msgWriter(std::string& msg, std::vector<std::string>& tokens) {
	std::stringstream s(msg, std::ios_base::out);
	std::vector<std::string>::iterator it;
	for (it = tokens.begin(); it != tokens.end(); it++)
		msg+= *it + "\n";
}
