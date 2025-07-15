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
#include <list>
#include <map>

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
	void update(std::list<Object*> &enemies, float deltaTime, bool &dead);
	void update(float deltaTime) override;
	void move(direction direction);
	void restart();

private:

	// Those are used to check collisions
	std::map<std::string, float> hitboxWidths;
	std::map<std::string, float> hitboxHeights;

	bool crawl;
	bool jumping;

	// Those are used for the jump physics
	float jump_velocity;
	float speedY;
	float gravity;

	bool isDead;
	
	// This is used to switch between the 2 legs to make the dino look like it runs
	unsigned int frameCount;

	std::map<std::string, GLuint> VAOs;
	std::map<std::string, GLuint> hitboxes;

	// Double hitbox for the standing sprite
	// The second one slightly translated from the first, to give them a Z-ish shape, like the dino
	glm::vec3 hitboxPosition[2];

	// This represent the translation from 1st to 2nd hitbox
	glm::vec3 offset;

};

#endif