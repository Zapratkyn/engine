#ifndef __WINDOW__
	#define __WINDOW__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window
{

public:

	static bool Init(int width, int height, const char *title);
	static bool ShouldClose();
	static void SwapBuffers();
	static void PollEvents();
	static void Shutdown();
	static GLFWwindow *GetGLFWwindow();

};

#endif