#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <glad/glad.h>

Shader *shader;
Mesh *mesh;

void Renderer::Init()
{
    shader = new Shader("shader.vert", "shader.frag");
	
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    mesh = new Mesh(vertices, sizeof(vertices));

}

void Renderer::Draw()
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader->use();
    mesh->Draw();
}
