#ifndef __INPUTMANAGER__
	#define __INPUTMANAGER__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace InputManager
{
	void HandleKey(int key, GLFWwindow *window);
	void ProcessInput();
}

#endif