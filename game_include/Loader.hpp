#ifndef __LOADER__
	#define __LOADER__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "data.hpp"
#include <string>
#include <iostream>

#include "../game_include/data.hpp"

class Loader
{

public:

	Loader(Scene scene, GLuint objectShader, GLuint hitboxShader, GLint positionLoc, GLint hitboxPositionLoc);
	~Loader();

	GLuint getObjectShader() const;
	GLuint getHitboxShader() const;
	GLuint getAssets() const;
	GLint getPositionLoc() const;
	GLint getHitboxPositionLoc() const;

	std::map<std::string, GLuint> getVAOs() const;
	std::map<std::string, GLuint> getHitboxes() const;

private:

	std::map<std::string, GLuint> VAOs;
	std::map<std::string, GLuint> hitboxes;
	std::vector<GLuint> VBOs;
	std::vector<GLuint> EBOs;

	GLuint objectShader;
	GLuint hitboxShader;

	GLint positionLoc;
	GLint hitboxPositionLoc;

	int texWidth;
	int texHeight;
	GLuint assets;

	void loadAssets(const char *assetsPath);
	void makeObject(std::string name, std::vector<int> &coords, std::map<std::string, std::vector<float>> &positions);
	void makeHitbox(std::string name, std::vector<float> &data);
	void calculateUVs(int x, int y, int w, int h, float uv[8]);


};

#endif