#ifndef __SCORE__
	#define __SCORE__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Object.hpp"
#include "Loader.hpp"
#include <list>

class Object;
class Loader;

class Score : public Object
{

public:

	Score(Loader *loader);

	void render();
	void update(bool &dead);

private:
	GLuint VAOs[10];
	GLuint hi;
	glm::vec3 hiPosition;
	glm::vec3 offsets[2];

	unsigned int currentScore;
	unsigned int hiScore;

	std::list<int> getDigits(unsigned int nb);

};


#endif