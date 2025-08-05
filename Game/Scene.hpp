#ifndef __SCENE__
	#define __SCENE__

#include "Player.hpp"
#include "../Helpers.h"
#include <string>
#include <unordered_map>
#include <utility>

namespace Scene
{
	void Load(std::string scene);
	void Update();
	void ShutDown();
	Player *getPlayer();
	std::string &getSceneName();
	std::unordered_map<std::string, struct Exit> &getExits();
}

#endif