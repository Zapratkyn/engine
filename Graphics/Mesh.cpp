#include "Mesh.hpp"
#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

Mesh::Mesh(float size)
{
	glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);

    float vertices[] =
    {
    	-size, -size,
    	 size, -size,
    	 size,  size,
    	-size,  size
    };

    float uvs[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    unsigned int indices[] =
    {
    	0, 1, 2,
    	2, 3, 0
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, &EBO);
}

void Mesh::UpdateUVs(const FrameUV& uvs)
{
    float uv[8] = 
    {
        uvs.u_min, uvs.v_min,
        uvs.u_max, uvs.v_min,
        uvs.u_max, uvs.v_max,
        uvs.u_min, uvs.v_max
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv), uv);
}

void Mesh::setVertices(float size)
{
    float v[] =
    {
        -size, -size,
         size, -size,
         size,  size,
        -size,  size
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
}

GLuint Mesh::getVAO() { return VAO; }
