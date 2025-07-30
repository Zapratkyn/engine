#ifndef __SCENE__
	#define __SCENE__

#include "Player.hpp"

namespace Scene
{
	void Load(const char* scene);
	void Update();
	Player *getPlayer();
	std::string &getSceneName();
}

#endif