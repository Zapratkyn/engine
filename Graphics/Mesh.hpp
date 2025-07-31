#ifndef __MESH__
	#define __MESH__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "../Helpers.h"

class Mesh
{

public:

	Mesh(float size);
	~Mesh();

	GLuint getVAO();
	void UpdateUVs(const struct FrameUV &uvs);
	void setVertices(float size);

private:

	GLuint VAO;
	GLuint VBOs[2];
	GLuint EBO;

};

#endif