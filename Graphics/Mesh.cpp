#include "Mesh.hpp"
#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

Mesh::Mesh(std::string path, int nbOfSprites, glm::vec3 pos)
{
	nbOfFrames = nbOfSprites;
	currentFrame = 0;
	frameDuration = 100;
	position = pos;

	lastUpdate = std::chrono::steady_clock::now();

	int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    glGenTextures(1, &sprites);
    glBindTexture(GL_TEXTURE_2D, sprites);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cerr << "ERROR::PLAYER::TEXTURE::LOADING_FAILED\n";
    stbi_image_free(data);

	GLuint vao[nbOfSprites], vbo[nbOfSprites], ebo[nbOfSprites];
	glGenVertexArrays(nbOfSprites, vao);
    glGenBuffers(nbOfSprites, vbo);
    glGenBuffers(nbOfSprites, ebo);

    unsigned int indices[] =
    {
    	0, 1, 2,
    	2, 3, 0
    };

    int spriteSize = width / nbOfSprites;

    for (int i = 0; i < nbOfSprites; i++)
    {
    	float u_min = (float)(spriteSize * i) / width;
	    float u_max = (float)(spriteSize * (i + 1)) / width;

	    float vertices[] =
	    {
	    	-0.3f, -0.3f, u_min, 0.0f,
	    	 0.3f, -0.3f, u_max, 0.0f,
	    	 0.3f,  0.3f, u_max, 1.0f,
	    	-0.3f,  0.3f, u_min, 1.0f
	    };

	    glBindVertexArray(vao[i]);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	    glEnableVertexAttribArray(1);

	    VAOs.push_back(vao[i]);
    	VBOs.push_back(vbo[i]);
    	EBOs.push_back(ebo[i]);
	}
}

Mesh::~Mesh()
{
	glDeleteBuffers(VBOs.size(), VBOs.data());
	glDeleteBuffers(EBOs.size(), EBOs.data());
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
}

void Mesh::Draw(GLint positionLoc)
{
	auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

    if (elapsed.count() >= frameDuration) 
    {
        currentFrame = (currentFrame + 1) % nbOfFrames;
        lastUpdate = now;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);

    glBindTexture(GL_TEXTURE_2D, sprites);
	glBindVertexArray(VAOs[currentFrame]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}