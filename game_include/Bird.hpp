#ifndef __BIRD__
	#define __BIRD__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Object.hpp"
#include "Loader.hpp"

class Object;
class Loader;

class Bird : public Object
{

public:

	Bird(Loader *loader);

	void render(bool showHitbox) override;

private:

	unsigned int frameCount;

	GLuint VAOs[2];
	GLuint hitbox;

};

#endif