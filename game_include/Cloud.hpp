#ifndef __CLOUD__
	#define __CLOUD__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Object.hpp"
#include "Loader.hpp"
#include <random>

class Object;
class Loader;

class Cloud : public Object
{

public:

	Cloud(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor);

};

#endif