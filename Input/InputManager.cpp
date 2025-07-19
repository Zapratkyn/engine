#include "InputManager.hpp"
#include "../Core/Window.hpp"

void InputManager::ProcessInput()
{
	GLFWwindow *window = Window::GetGLFWwindow();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}