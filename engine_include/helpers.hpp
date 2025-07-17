#ifndef __HELPERS__
	#define __HELPERS__

#include <iostream>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

void makeShader(const char *vertexPath, const char *fragmentPath, GLuint *shader, const char *subType);
void checkCompileErrors(unsigned int shader, std::string type, const char *subType);

#endif