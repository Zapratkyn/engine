#ifndef __INPUTMANAGER__
	#define __INPUTMANAGER__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../Game/Player.hpp"
#include "../Graphics/Helpers.h"

const int allKeys[] = 
{
	GLFW_KEY_ESCAPE, 
	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_RIGHT,
	// GLFW_KEY_DOWN,
	GLFW_KEY_LEFT,
	// GLFW_KEY_UP,
	GLFW_KEY_F1,
	GLFW_KEY_E,
	GLFW_KEY_Q
};

namespace InputManager
{
	void HandleKeyDown(int key, GLFWwindow *window, Player *player);
	void HandleKeyUp(int key, GLFWwindow *window, Player *player);
	void ProcessInput();
}

#endif