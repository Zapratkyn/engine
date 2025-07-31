#include "Core/Engine.hpp"
#include "Core/Config.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	(void)argv;
	if (argc > 1)
	{
		std::cerr << "ERROR\nNo option available" << std::endl;
		return 1;
	}

	initConfig(1920, 1080);

	Engine engine;

	if (engine.Init())
		engine.Run();

	engine.ShutDown();
	
	return 0;
}
