#include "Window.hpp"
#include "../Input/InputManager.hpp"
#include "Config.hpp"
#include <glad/glad.h>

static GLFWwindow* window = nullptr;

bool Window::Init(const char *title)
{
    std::unordered_map config = getConfig();
	if (!glfwInit()) return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(config["width"], config["height"], title, nullptr, nullptr);
    if (!window) return false;
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;
    glViewport(0, 0, config["width"], config["height"]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void Window::SwapBuffers() { glfwSwapBuffers(window); }
void Window::PollEvents() { glfwPollEvents(); }
bool Window::ShouldClose() { return glfwWindowShouldClose(window); }
void Window::Shutdown() 
{ 
    glfwDestroyWindow(window); 
    glfwTerminate(); 
}

GLFWwindow* Window::GetGLFWwindow() { return window; }