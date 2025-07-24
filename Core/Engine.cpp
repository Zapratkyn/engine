#include "Engine.hpp"
#include "Window.hpp"
#include "../Input/InputManager.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Game/Scene.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

bool Engine::Init()
{
	Window::Init(800, 600, "Mini engine");
	Renderer::Init();
	Scene::Load("test");
	return true;
}

void Engine::Run()
{
	Scene::getPlayer()->StartAnimation();
	while (!Window::ShouldClose())
	{
		Renderer::Draw();
		InputManager::ProcessInput();
		Window::PollEvents();
		Window::SwapBuffers();
	}
}

void Engine::ShutDown() 
{ 
	Renderer::Free();
	Window::Shutdown();
}
