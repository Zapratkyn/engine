#ifndef __SHADER__
	#define __SHADER__

#include <string>

class Shader
{

public:

	Shader(const char *vPath, const char *fPath);

	unsigned int ID;

	void use();

};

#endif