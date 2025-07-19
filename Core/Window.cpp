#include "Window.hpp"
#include <glad/glad.h>

static GLFWwindow* window = nullptr;

bool Window::Init(int width, int height, const char *title)
{
	if (!glfwInit()) return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) return false;
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;
    glViewport(0, 0, width, height);
    return true;
}

void Window::SwapBuffers() { glfwSwapBuffers(window); }
void Window::PollEvents() { glfwPollEvents(); }
void Window::Shutdown() { glfwDestroyWindow(window); glfwTerminate(); }
bool Window::ShouldClose() { return glfwWindowShouldClose(window); }
GLFWwindow* Window::GetGLFWwindow() { return window; }
