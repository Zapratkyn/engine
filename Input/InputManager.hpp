#ifndef __INPUTMANAGER__
	#define __INPUTMANAGER__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../Game/Player.hpp"

namespace InputManager
{
	void HandleKey(int key, GLFWwindow *window, Player *player);
	void ProcessInput();
}

#endif