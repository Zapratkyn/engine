#include "Renderer.hpp"
#include "../Game/Scene.hpp"
#include "../Core/Config.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include "stb_image.h"


/*
"Lazy" declaration of static objects
I need them empty upon construction
*/

std::unordered_map<std::string, Shader*> &Renderer::getShaders()
{
    static std::unordered_map<std::string, Shader*> shaders;
    return shaders;
}

std::unordered_map<std::string, std::unordered_map<std::string, struct Animation*>> &Renderer::getAnimations()
{
    static std::unordered_map<std::string, std::unordered_map<std::string, struct Animation*>> animations;
    return animations;
}

std::unordered_map<std::string, struct Texture*> &Renderer::getTextures()
{
    static std::unordered_map<std::string, struct Texture*> textures;
    return textures;
}

// std::unordered_map<std::string, std::unordered_map<std::string, struct Terrain*>> &Renderer::getTerrains()
// {
//     static std::unordered_map<std::string, std::unordered_map<std::string, struct Terrain*>> animations;
//     return animations;
// }

std::unordered_map<std::string, Mesh*> &Renderer::getMeshes()
{
    static std::unordered_map<std::string, Mesh*> meshes;
    return meshes;
}

static GLint modelLoc, colorLoc, projectionLoc, scaleLoc, scrollOffsetLoc;
static json data;
struct Background background;
glm::mat4 projection;


struct Texture *ParseTexture(std::string &texName, std::string &texSource);
struct Animation *ParseAnim(std::string &animName, json::iterator::reference &animData, std::unordered_map<std::string, struct Texture*> &textures);
struct Background ParseBg(struct Texture *texture);

void Renderer::Init()
{
    std::ifstream file("Graphics/data.json");
    data = json::parse(file);

    for (auto& [texName, texSource] : data["textures"].items())
    {
        std::string name(texName), source(texSource);
        getTextures()[name] = ParseTexture(name, source);
    }

    for (auto& [unitName, unitData] : data["animations"].items())
    {
        std::string name(unitName);
        getAnimations().try_emplace(name);
        for (auto& [animName, animData] : unitData.items())
        {
            std::string aName(animName);
            getAnimations()[name][aName] = ParseAnim(aName, animData, getTextures());
        }
    }
	
    getShaders()["unit"] = new Shader("unit.vert", "unit.frag");
    getShaders()["background"] = new Shader("bg.vert", "bg.frag");

    modelLoc = glGetUniformLocation(getShaders()["unit"]->ID, "model");
    projectionLoc = glGetUniformLocation(getShaders()["unit"]->ID, "projection");
    colorLoc = glGetUniformLocation(getShaders()["unit"]->ID, "backgroundColor");
    scaleLoc = glGetUniformLocation(getShaders()["background"]->ID, "scale");
    scrollOffsetLoc = glGetUniformLocation(getShaders()["background"]->ID, "scrollOffset");

    projection = glm::ortho(
        0.0f,
        (float)800,
        0.0f,
        (float)600
    );

    getMeshes()["unit"] = new Mesh(0.5f);
    getMeshes()["background"] = new Mesh(1.0f);

    std::string bgName(data["scenes"]["test"]["background"]);
    background = ParseBg(getTextures()[bgName]);
}

struct Texture *ParseTexture(std::string &texName, std::string &texSource)
{
    struct Texture *tex = new struct Texture;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(texSource.c_str(), &tex->width, &tex->height, &channels, STBI_rgb_alpha);

    glGenTextures(1, &tex->texture);
    glBindTexture(GL_TEXTURE_2D, tex->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cerr << "ERROR::TEXTURE::LOADING_FAILED::" << texName << "\n";
    stbi_image_free(imageData);

    return tex;
}

struct Animation *ParseAnim(std::string &animName, json::iterator::reference &animData, std::unordered_map<std::string, struct Texture*> &textures)
{
    struct Animation *anim = new struct Animation;
    std::string texSource(animData["texture"]);

    anim->name = animName;
    anim->texture = textures[texSource];
    anim->frameWidth = animData["frameWidth"];
    anim->frameHeight = animData["frameHeight"];
    anim->frameTime = animData["frameTime"];
    anim->frameCount = animData["frameCount"];

    int cols = animData["columns"].get<int>();
    int startX = animData["startX"].get<int>(); 
    int startY = animData["startY"].get<int>();

    for (int i = 0; i < animData["frameCount"]; ++i) {
        int x = startX + (i % cols);
        int y = startY + (i / cols);

        float u_min = (x * anim->frameWidth) / (float)anim->texture->width;
        float u_max = ((x + 1) * anim->frameWidth) / (float)anim->texture->width;
        float v_max = 1.0f - (y * anim->frameHeight) / (float)anim->texture->height;
        float v_min = v_max - (anim->frameHeight / (float)anim->texture->height);

        anim->frames.push_back({u_min, v_min, u_max, v_max});
    }

    return anim;
}

struct Background ParseBg(struct Texture *texture)
{
    struct Background bg;
    bg.texture = texture;

    std::unordered_map config = getConfig();
    bg.scaleX = (float)texture->width / texture->height;
    bg.scaleY = (float)config["width"] / config["height"];

    return bg;
}

void Renderer::Draw()
{
    // Drawing background
    getShaders()["background"]->use();
    glUniform2f(scrollOffsetLoc, 0, -0.15);
    glUniform2f(scaleLoc, background.scaleX, background.scaleY);
    glBindTexture(GL_TEXTURE_2D, background.texture->texture);
    glBindVertexArray(getMeshes()["background"]->getVAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Drawing units
    getShaders()["unit"]->use();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    Scene::getPlayer()->Draw();
}

void Renderer::Free()
{
    for (auto it = getShaders().begin(); it != getShaders().end(); it++)
        delete it->second;

    for (auto it = getAnimations().begin(); it != getAnimations().end(); it++)
    {
        for (auto anim = it->second.begin(); anim != it->second.end(); anim++)
            delete anim->second;
    }

    for (auto it = getTextures().begin(); it != getTextures().end(); it++)
        delete it->second;

    for (auto it = getMeshes().begin(); it != getMeshes().end(); it++)
        delete it->second;
}

GLint Renderer::getModelLoc() { return modelLoc; }
GLint Renderer::getColorLoc() { return colorLoc; }
json Renderer::getData() { return data; }
