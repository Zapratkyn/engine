#include "InputManager.hpp"
#include "../Core/Window.hpp"
#include "../Game/Scene.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, bool> keyPressed;

void InputManager::HandleKeyDown(int key, GLFWwindow *window, Player *player)
{
	switch (key) {
        case GLFW_KEY_LEFT_SHIFT:
            player->setGuarding(true);
            break;
        case GLFW_KEY_RIGHT:
            player->setMoving(RIGHT, true);
            break;
        case GLFW_KEY_DOWN:
            player->setMoving(DOWN, true);
            break;
        case GLFW_KEY_LEFT:
            player->setMoving(LEFT, true);
            break;
        case GLFW_KEY_UP:
            player->setMoving(UP, true);
            break;
        case GLFW_KEY_M:
            player->ToggleMeshDisplay();
            break;
        case GLFW_KEY_A:
            player->Attack("1");
            break;
        case GLFW_KEY_E:
            player->Attack("2");
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            std::cout << "This key has no effect" << std::endl;
            break;
    }
}

void InputManager::HandleKeyUp(int key, GLFWwindow *window, Player *player)
{
    (void)window;
    switch (key) {
        case GLFW_KEY_LEFT_SHIFT:
            player->setGuarding(false);
            break;
        case GLFW_KEY_RIGHT:
            player->setMoving(RIGHT, false);
            break;
        case GLFW_KEY_DOWN:
            player->setMoving(DOWN, false);
            break;
        case GLFW_KEY_LEFT:
            player->setMoving(LEFT, false);
            break;
        case GLFW_KEY_UP:
            player->setMoving(UP, false);
            break;
        default:
            // std::cout << "This key has no effect" << std::endl;
            break;
    }
}


void InputManager::ProcessInput()
{
	GLFWwindow *window = Window::GetGLFWwindow();
    Player *player = Scene::getPlayer();
	for (int key : allKeys)
	{
        bool isDown = glfwGetKey(window, key) == GLFW_PRESS;

        if (isDown && !keyPressed[key]) {
            HandleKeyDown(key, window, player);
            keyPressed[key] = true;
        }

        if (!isDown && keyPressed[key]) {
            HandleKeyUp(key, window, player);
            keyPressed[key] = false;
        }
    }
}
