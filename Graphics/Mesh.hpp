#ifndef __MESH__
	#define __MESH__

#include <glad/glad.h>
#include <string>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh
{

public:

	Mesh(std::string path, int nbOfSprites, glm::vec3 pos);
	~Mesh();
	void Draw(GLint positionLoc);

private:

	std::vector<GLuint> VAOs;
	std::vector<GLuint> VBOs;
	std::vector<GLuint> EBOs;
	GLuint sprites;

	size_t nbOfFrames;
	int frameDuration;
	size_t currentFrame;

	std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

	glm::vec3 position;

};

#endif