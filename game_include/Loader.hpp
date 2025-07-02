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

	Loader(Scene scene, GLuint objectShader, GLuint hitboxShader, GLint positionLoc, GLint hitboxPositionLoc, GLint hitboxColorLoc);
	~Loader();

	GLuint getObjectShader() const;
	GLuint getHitboxShader() const;
	GLuint getAssets() const;
	GLint getPositionLoc() const;
	GLint getHitboxPositionLoc() const;
	GLint getHitboxColorLoc() const;

	std::map<std::string, GLuint> getVAOs() const;

private:

	// std::map<std::string, std::vector<int>> data;
	std::map<std::string, GLuint> VAOs;
	std::vector<GLuint> VBOs;
	std::vector<GLuint> EBOs;

	GLuint objectShader;
	GLuint hitboxShader;

	GLint positionLoc;
	GLint hitboxPositionLoc;
	GLint hitboxColorLoc;

	int texWidth;
	int texHeight;
	GLuint assets;

	void loadAssets(const char *assetsPath);
	void makeObject(std::string name, std::vector<int> &coords, std::map<std::string, std::vector<float>> &positions);
	void calculateUVs(int x, int y, int w, int h, float uv[8]);


};

#endif