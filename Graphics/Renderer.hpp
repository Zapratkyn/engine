#ifndef __RENDERER__
	#define __RENDERER__

#include "Mesh.hpp"
#include <unordered_map>
#include <string>

namespace Renderer
{
	std::unordered_map<std::string, Mesh*> &getMeshes();
	void Init();
	void Draw();
}

#endif