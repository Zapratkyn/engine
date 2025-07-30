#include "Core/Engine.hpp"
#include "Core/Config.hpp"

int main()
{
	initConfig(800, 600);

	Engine engine;

	if (engine.Init())
		engine.Run();

	engine.ShutDown();
	
	return 0;
}
