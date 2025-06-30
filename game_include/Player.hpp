#ifndef __PLAYER__
	#define __PLAYER__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../game_include/stb_image.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum movement {
	DOWN,
	UP,
	JUMP
};

enum press {
	ON,
	OFF
};

class Player
{
public:

	Player(GLuint objectShader, GLuint hitboxShader);

	void render();
	void move(movement direction, float deltaTime);

	void toggleHitbox(press button);

private:

	bool crawl;

	glm::vec3 position;
	GLint positionLoc;

	GLuint VAO[4];
	GLuint shader;
	GLuint texture;

	GLuint hitbox[2];
	GLuint hitboxShader;
	GLint hitboxPositionLoc;
	GLint hitboxColorLoc;

	bool showHitbox;

	unsigned int frameCount;

	void makeObject();
	void makeHitbox();
	void calculateUVs(int x, int y, int w, int h, int texWidth, int texHeight, float uv[8]);

};

#endif