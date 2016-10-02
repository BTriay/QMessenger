#include "Bundle.h"

Bundle::Bundle() {
	bool conf_threads = false;
	bool conf_port = false;
	bool conf_db = false;

	std::vector<std::string> configTokens;
	if (configFileReader("config", configTokens)) {
		for (std::vector<std::string>::const_iterator it = configTokens.begin(); it != configTokens.end(); it++) {
			if (*it == CONFIG_DB) {
				++it;
				std::ifstream f((*it).c_str(), std::ios_base::in);
				a_db = new Db(*it, !f.good());
				conf_db = true;
			}
			else if (*it == CONFIG_THREADS) {
				++it;
				a_threadNo = stoi(*it);
				conf_threads = true;
			}
			else if (*it == CONFIG_PORT) {
				++it;
				a_portNo = *it;
				conf_port = true;
			}
		}
	}
//if config file does not exist or is missing some info, use default configuration
	if (!conf_threads)
		a_threadNo = CONFIG_THREADS_DEF;
	if (!conf_port)
		a_portNo = CONFIG_PORT_DEF;
	if (!conf_db) {
		std::ifstream f(CONFIG_DB_DEF, std::ios_base::in);
		a_db = new Db(CONFIG_DB_DEF, !f.good());
	}

	a_matrix = new MatrixServer();
	a_epfd = epoll_create1(0);
std::cout << "bundle constructor: " << a_epfd << "\n";
}

Bundle::~Bundle() {
std::cout << "bundle destructor\n";
	delete a_db;
	delete a_matrix;
}
