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

	bool crawl;
	std::map<std::string, float> hitboxWidths;
	std::map<std::string, float> hitboxHeights;

	bool jumping;
	float jump_velocity;
	float speedY;
	float gravity;

	bool isDead;
	
	unsigned int frameCount;

	std::map<std::string, GLuint> VAOs;
	std::map<std::string, GLuint> hitboxes;
	glm::vec3 hitboxPosition[2];
	glm::vec3 offset;

};

#endif