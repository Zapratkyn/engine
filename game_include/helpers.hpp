#ifndef __HELPERS__
	#define __HELPERS__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

GLuint makeShader(const char *vertexPath, const char *fragmentPath, std::string object);
// GLuint makeHitbox(glm::vec3 color);
void checkCompileErrors(unsigned int shader, std::string type, std::string object);

#endif