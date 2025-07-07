#ifndef __LANDSCAPE__
	#define __LANDSCAPE__

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

class Ground : public Object
{
public:

	Ground(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor);

	void render(bool showHitbox) override;
	void update(float deltaTime) override;

};

#endif