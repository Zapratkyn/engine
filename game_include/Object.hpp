#ifndef __OBJECT__
	#define __OBJECT__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Loader.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Loader;

class Object
{
public:

	Object(Loader *loader);

	virtual void render(bool showHitbox) = 0;

protected:

	GLuint objectShader;
	GLuint hitboxShader;
	GLuint assets;

	GLint positionLoc;
	GLint hitboxPositionLoc;
	GLint hitboxColorLoc;

	glm::vec3 position;
	glm::vec3 color;

};

#endif