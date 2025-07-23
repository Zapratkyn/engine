#include "Renderer.hpp"
#include "Shader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader *shader;
GLint positionLoc;

std::unordered_map<std::string, Mesh*> &Renderer::getMeshes()
{
    static std::unordered_map<std::string, Mesh*> meshes;
    return meshes;
}

void Renderer::Init()
{
    std::string assetsPath("Resources/Tiny Swords/");

    getMeshes()["black_warrior_idle"] = new Mesh(assetsPath + "Units/Black Units/Warrior/Warrior_Idle.png", 8, glm::vec3(-0.5f, 0.0f, 0.0f));
    getMeshes()["black_monk_idle"] = new Mesh(assetsPath + "Units/Black Units/Monk/Idle.png", 6, glm::vec3(0.5f, 0.0f, 0.0f));
    getMeshes()["yellow_lancer_idle"] = new Mesh(assetsPath + "Units/Yellow Units/Lancer/Lancer_Idle.png", 12, glm::vec3(0.0f, 0.5f, 0.0f));
	
    shader = new Shader("shader.vert", "shader.frag");
    positionLoc = glGetUniformLocation(shader->ID, "position");
}

void Renderer::Draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader->use();
    for (auto it = getMeshes().begin(); it != getMeshes().end(); it++)
        it->second->Draw(positionLoc);
}
