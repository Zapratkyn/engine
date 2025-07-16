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
	Object(Loader *loader, glm::vec3 movement, float speed);
	virtual ~Object() = default;

	virtual void render(bool showHitbox);
	virtual void render();
	virtual void update(float deltaTime);
	glm::vec3 getMin();
	glm::vec3 getMax();

protected:

	GLuint objectShader;
	GLuint hitboxShader;
	GLuint assets;

	GLuint VAO;
	GLuint hitbox;

	GLint positionLoc;
	GLint hitboxPositionLoc;

	glm::vec3 position;
	float width;
	float height;

	glm::vec3 movement;
	float speed;

	unsigned int frameCount;

};

#endif