#include "Config.hpp"

std::unordered_map<std::string, int> globalConfig;

void initConfig(int width, int height)
{
	globalConfig["width"] = width;
	globalConfig["height"] = height;
}

std::unordered_map<std::string, int> &getConfig() { return globalConfig; }