#include "Renderer.hpp"
#include "../Game/Scene.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include "stb_image.h"


std::unordered_map<std::string, Shader*> &Renderer::getShaders()
{
    static std::unordered_map<std::string, Shader*> shaders;
    return shaders;
}

std::unordered_map<std::string, struct Animation*> &Renderer::getAnimations()
{
    static std::unordered_map<std::string, struct Animation*> animations;
    return animations;
}

std::unordered_map<std::string, Mesh*> &Renderer::getMeshes()
{
    static std::unordered_map<std::string, Mesh*> meshes;
    return meshes;
}

static GLint modelLoc, colorLoc, projectionLoc;
static json data;

struct Animation *ParseAnims(std::string &animName, json::iterator::reference &animData);

void Renderer::Init()
{
    std::ifstream file("Graphics/data.json");
    data = json::parse(file);

    for (auto& [animName, animData] : data["animations"].items())
    {
        std::string name(animName);
        getAnimations()[name] = ParseAnims(name, animData);
    }
	
    getShaders()["main"] = new Shader("shader.vert", "shader.frag");
    getShaders()["main"]->use();
    modelLoc = glGetUniformLocation(getShaders()["main"]->ID, "model");
    projectionLoc = glGetUniformLocation(getShaders()["main"]->ID, "projection");
    colorLoc = glGetUniformLocation(getShaders()["main"]->ID, "backgroundColor");

    glm::mat4 projection = glm::ortho(
        0.0f,               // gauche
        (float)800,         // droite
        0.0f,               // bas
        (float)600          // haut
    );

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    getMeshes()["unit"] = new Mesh;
}

struct Animation *ParseAnims(std::string &animName, json::iterator::reference &animData)
{
    struct Animation *anim = new struct Animation;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    std::string path = animData["texture"];
    unsigned char* imageData = stbi_load(path.c_str(), &anim->textureWidth, &anim->textureHeight, &channels, STBI_rgb_alpha);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, anim->textureWidth, anim->textureHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cerr << "ERROR::TEXTURE::LOADING_FAILED::" << animName << "\n";
    stbi_image_free(imageData);

    anim->name = animName;
    anim->texture = texture;
    anim->frameWidth = animData["frameWidth"];
    anim->frameHeight = animData["frameHeight"];
    anim->frameCount = animData["frameCount"];
    anim->frameTime = animData["frameTime"];
    anim->framesPerRow = anim->textureWidth / anim->frameWidth;

    return anim;
}

void Renderer::Draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Scene::getPlayer()->Draw();
}

void Renderer::Free()
{
    for (auto it = getShaders().begin(); it != getShaders().end(); it++)
        delete it->second;

    for (auto it = getAnimations().begin(); it != getAnimations().end(); it++)
        delete it->second;

    for (auto it = getMeshes().begin(); it != getMeshes().end(); it++)
        delete it->second;
}

GLint Renderer::getModelLoc() { return modelLoc; }
GLint Renderer::getColorLoc() { return colorLoc; }
json Renderer::getData() { return data; }
