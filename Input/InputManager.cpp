#include "InputManager.hpp"
#include "../Core/Window.hpp"
#include "../Game/Scene.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, bool> keyPressed;

void InputManager::HandleKey(int key, GLFWwindow *window, Player *player)
{
	switch (key) {
        case GLFW_KEY_LEFT_SHIFT:
            if (!player->isGuarding())
                player->setGuarding(true);
            break;
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
    Player *player = Scene::getPlayer();
	for (int key : {GLFW_KEY_P, GLFW_KEY_G, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT_SHIFT}) 
	{
        bool isDown = glfwGetKey(window, key) == GLFW_PRESS;

        if (isDown && !keyPressed[key]) {
            HandleKey(key, window, player);
            keyPressed[key] = true;
        }

        if (!isDown && keyPressed[key]) {
            keyPressed[key] = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS && player->isGuarding())
        player->setGuarding(false);
}
