#include "Engine.hpp"
#include "Window.hpp"
#include "../Input/InputManager.hpp"
#include "../Graphics/Renderer.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

bool Engine::Init()
{
	Window::Init(800, 600, "Mini engine");
	Renderer::Init();
	return true;
}

void Engine::Run()
{
	while (!Window::ShouldClose())
	{
		Renderer::Draw();
		InputManager::ProcessInput();
		Window::PollEvents();
		Window::SwapBuffers();
	}
}

void Engine::ShutDown() { Window::Shutdown(); }
