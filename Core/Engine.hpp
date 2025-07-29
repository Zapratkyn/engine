#ifndef __ENGINE__
	#define __ENGINE__

#include <string>

class Engine
{

public:

	bool Init(std::string &unit);
	void Run();
	void ShutDown();

};

#endif