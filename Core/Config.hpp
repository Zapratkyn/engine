#ifndef __CONFIG__
	#define __CONFIG__

#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, int> globalConfig;

void initConfig(int width, int height);
std::unordered_map<std::string, int> &getConfig();

#endif