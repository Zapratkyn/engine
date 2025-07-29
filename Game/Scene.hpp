#ifndef __SCENE__
	#define __SCENE__

#include "Player.hpp"

namespace Scene
{
	void Load(const char* scene, std::string &unit);
	void Update();
	Player *getPlayer();
}

#endif