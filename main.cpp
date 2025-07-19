#include "Core/Engine.hpp"

int main()
{
	Engine engine;

	if (engine.Init())
		engine.Run();

	engine.ShutDown();
	
	return 0;
}