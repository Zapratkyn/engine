#ifndef __MESH__
	#define __MESH__

#include <glad/glad.h>

class Mesh
{

public:

	Mesh(const float *vertices, int size);
	~Mesh();
	void Draw() const;

private:

	GLuint VAO, VBO;

};

#endif