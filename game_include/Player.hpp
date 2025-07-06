#ifndef __PLAYER__
	#define __PLAYER__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Object.hpp"
#include "Loader.hpp"

enum direction {
	CROUCH,
	STAND,
	JUMP
};

class Object;
class Loader;

class Player : public Object
{
public:

	Player(Loader *loader);

	void render(bool showHitbox) override;
	void update(float deltaTime) override;
	void move(direction direction);

private:

	bool crawl;

	bool jumping;
	float jump_velocity;
	float speedY;
	float gravity;
	
	unsigned int frameCount;

	GLuint VAOs[5];
	GLuint hitbox[2];

	void makeHitboxes();

};

#endif