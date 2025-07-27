#include "Mesh.hpp"
#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

Mesh::Mesh()
{
	glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);

    float vertices[] =
    {
    	-0.5f, -0.5f,
    	 0.5f, -0.5f,
    	 0.5f,  0.5f,
    	-0.5f,  0.5f
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

    indexCount = 6;

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

void Mesh::UpdateUVs(const std::vector<float>& uvs)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, uvs.size() * sizeof(float), uvs.data());
}

GLuint Mesh::getVAO() { return VAO; }
