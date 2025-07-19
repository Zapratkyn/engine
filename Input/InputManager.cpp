#include "InputManager.hpp"
#include "../Core/Window.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, bool> keyPressed;

void InputManager::HandleKey(int key, GLFWwindow *window)
{
	switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            std::cout << "This key has no effect" << std::endl;
            break;
    }
}


void InputManager::ProcessInput()
{
	GLFWwindow *window = Window::GetGLFWwindow();
	for (int key : {GLFW_KEY_P, GLFW_KEY_G, GLFW_KEY_ESCAPE}) 
	{
        bool isDown = glfwGetKey(window, key) == GLFW_PRESS;

        if (isDown && !keyPressed[key]) {
            HandleKey(key, window);
            keyPressed[key] = true;
        }

        if (!isDown && keyPressed[key]) {
            keyPressed[key] = false;
        }
    }
}