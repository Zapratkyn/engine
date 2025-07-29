#include "Core/Engine.hpp"
#include <iostream>

bool checkUnit(std::string &unit);

int main(int argc, char **argv)
{

	std::string unit("warrior");

	if (argc > 2)
	{
		std::cerr << "ERROR:\nExpected ./build/engine [unit for the player]" << std::endl;
		return 1;
	}

	if (argc == 2)
	{
		unit = argv[1];
		if (!checkUnit(unit))
		{
			std::cerr << "ERROR:\nUnknown unit" << std::endl;
			return 1;
		}
	}

	Engine engine;

	if (engine.Init(unit))
		engine.Run();

	engine.ShutDown();
	
	return 0;
}

bool checkUnit(std::string &unit)
{
	const std::string units[] = 
	{
		"warrior",
		"archer",
		"lancer",
		"monk"
	};

	for (std::string u : units)
	{
		if (u == unit)
			return true;
	}

	return false;
}