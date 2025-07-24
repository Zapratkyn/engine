#ifndef __MESH__
	#define __MESH__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Mesh
{

public:

	Mesh();
	~Mesh();

	GLuint getVAO();
	void UpdateUVs(const std::vector<float>& uvs);

private:

	GLuint VAO;
	GLuint VBOs[2];
	GLuint EBO;
	GLsizei indexCount;

};

#endif