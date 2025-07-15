#ifndef __RESTART__
	#define __RESTART__

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

class Restart : public Object
{

public:

	Restart(Loader *loader);

	void render(bool showHitbox) override;
	void update(float deltaTime) override;

};

#endif