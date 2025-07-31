#include "Renderer.hpp"
#include "../Game/Scene.hpp"
#include "../Core/Config.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "stb_image.h"


/*
"Lazy" declaration of static objects
I need them empty upon construction
*/


/*
One shader for the units (position and projection)
Another one for the background (scale and offset)
*/
std::unordered_map<std::string, Shader*> &Renderer::getShaders()
{
    static std::unordered_map<std::string, Shader*> shaders;
    return shaders;
}

// unordered_map<Unit's name, unordered_map<Animation's name, List of sprites (see Helpers.h)>>
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

/*
2 vectors for the decor
"back" for background, aesthetic only
"front" for the tiles. Those need collision detection
*/
std::unordered_map<std::string, std::vector<struct Texture*>> &Renderer::getDecors()
{
    static std::unordered_map<std::string, std::vector<struct Texture*>> decors;
    return decors;
}

/*
One mesh for the units (can move, fraction of the screen)
Another one for the background (cannot move, whole screen)
*/
std::unordered_map<std::string, Mesh*> &Renderer::getMeshes()
{
    static std::unordered_map<std::string, Mesh*> meshes;
    return meshes;
}

// Uniforms
static GLint modelLoc, colorLoc, projectionLoc, scaleLoc, scrollOffsetLoc;
static json data;
glm::mat4 projection;


struct Texture *ParseTexture(std::string texName, std::string texSource, std::string type);
struct Animation *ParseAnim(std::string animName, json::iterator::reference &animData, std::unordered_map<std::string, struct Texture*> &textures);

void Renderer::Init()
{
    std::ifstream file("Game/data.json");
    data = json::parse(file);
	
    getShaders()["unit"] = new Shader("unit.vert", "unit.frag");
    getShaders()["background"] = new Shader("bg.vert", "bg.frag");

    modelLoc = glGetUniformLocation(getShaders()["unit"]->ID, "model");
    projectionLoc = glGetUniformLocation(getShaders()["unit"]->ID, "projection");
    colorLoc = glGetUniformLocation(getShaders()["unit"]->ID, "backgroundColor");
    scaleLoc = glGetUniformLocation(getShaders()["background"]->ID, "scale");
    scrollOffsetLoc = glGetUniformLocation(getShaders()["background"]->ID, "scrollOffset");

    auto config = getConfig();
    projection = glm::ortho(0.0f, (float)config["width"], 0.0f, (float)config["height"]);

    getMeshes()["unit"] = new Mesh(0.5f);
    getMeshes()["background"] = new Mesh(1.0f);
}

void Renderer::ParseScene(const char *scene)
{
    // We load only the textures we need for the scene

    auto sceneToLoad = data["scenes"][scene];

    std::vector<std::string> texturesToLoad;
    std::vector<std::string> back;

    texturesToLoad.push_back(sceneToLoad["player"]["unit"]);

    /*
    For each unit type in the scene, we get the name
    Then, we use the unit's name to iterate through its animations and push those names in the list
    We avoid duplicates thanks to std::find
    */
    for (auto &[unitName, unitData] : sceneToLoad["units"].items())
    {
        for (auto& [uName, uData] : data["animations"][unitName].items())
        {
            std::string texName(uData["texture"]);
            if (std::find(texturesToLoad.begin(), texturesToLoad.end(), texName) == texturesToLoad.end())
                texturesToLoad.push_back(texName);
        }
    }

    /*
    Get the name of every layer of the scene's decor
    We start with the background. Those layers don't need collision detection
    */
    for (const auto &decorName : data["decors"][sceneToLoad["decor"]]["back"])
    {
        texturesToLoad.push_back(decorName);
        back.push_back(decorName);
    }

    // Then the foreground. This needs collision detection
    for (const auto &decorName : data["decors"][sceneToLoad["decor"]]["front"])
        texturesToLoad.push_back(decorName);

    // Parsing all the animations, according to the list we made before
    for (auto& [texName, texSource] : data["animations_textures"].items())
    {
        if (std::find(texturesToLoad.begin(), texturesToLoad.end(), texName) != texturesToLoad.end())
            getTextures()[texName] = ParseTexture(texName, texSource, "animation");
    }

    /*
    Parsing all the decors, according to the list we made before
    Putting the textures in the correct vector, depending on the need of collision detection
    Erasing each parsed texture from the list to avoid duplicate
    */
    for (auto& [texName, texSource] : data["decors_textures"].items())
    {
        auto it = std::find(texturesToLoad.begin(), texturesToLoad.end(), texName);
        if (it == texturesToLoad.end())
            continue;
        auto itBack = std::find(back.begin(), back.end(), texName);
        if (itBack != back.end())
        {
            getDecors()["back"].push_back(ParseTexture(texName, texSource, "decor"));
            itBack = back.erase(itBack);
        }
        else
            getDecors()["front"].push_back(ParseTexture(texName, texSource, "decor"));
        it = texturesToLoad.erase(it);
    }

    /*
    Parsing all the animations, according to the list we made before
    Animations are organized in a map of maps
    Each unit is an element in the map of maps
    Each unit has a map of animations
    Erasing each parsed animation from the list to avoid duplicate
    */
    for (auto& [unitName, unitData] : data["animations"].items())
    {
        auto it = std::find(texturesToLoad.begin(), texturesToLoad.end(), unitName);
        if (it == texturesToLoad.end())
            continue;
        getAnimations().try_emplace(unitName);
        for (auto& [animName, animData] : unitData.items())
            getAnimations()[unitName][animName] = ParseAnim(animName, animData, getTextures());
        it = texturesToLoad.erase(it);
    }
}

struct Texture *ParseTexture(std::string texName, std::string texSource, std::string type)
{
    struct Texture *tex = new struct Texture;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(texSource.c_str(), &tex->width, &tex->height, &channels, STBI_rgb_alpha);

    glGenTextures(1, &tex->texture);
    glBindTexture(GL_TEXTURE_2D, tex->texture);

    if (type == "animation")
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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

struct Animation *ParseAnim(std::string animName, json::iterator::reference &animData, std::unordered_map<std::string, struct Texture*> &textures)
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

void Renderer::Draw()
{
    // DEPRECATED
    // Drawing background
    // getShaders()["background"]->use();
    // glUniform2f(scrollOffsetLoc, 0, -0.15);
    // glUniform2f(scaleLoc, background.scaleX, background.scaleY);
    // glBindTexture(GL_TEXTURE_2D, background.texture->texture);
    // glBindVertexArray(getMeshes()["background"]->getVAO());
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Back to simple color background until I make the background properly using all the layers loaded in the getDecors() vector
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing units
    getShaders()["unit"]->use();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    Scene::getPlayer()->Draw();
}

void Renderer::Free()
{
    for (auto it = getAnimations().begin(); it != getAnimations().end(); it++)
    {
        for (auto anim = it->second.begin(); anim != it->second.end(); anim++)
            delete anim->second;
    }

    for (auto it = getTextures().begin(); it != getTextures().end(); it++)
        delete it->second;

    for (auto it = getDecors().begin(); it != getDecors().end(); it++)
    {
        for (auto tex = it->second.begin(); tex != it->second.end(); tex++)
            delete *tex;
    }
}

void Renderer::ShutDown()
{
    for (auto it = getShaders().begin(); it != getShaders().end(); it++)
        delete it->second;

    for (auto it = getAnimations().begin(); it != getAnimations().end(); it++)
    {
        for (auto anim = it->second.begin(); anim != it->second.end(); anim++)
            delete anim->second;
    }

    for (auto it = getDecors().begin(); it != getDecors().end(); it++)
    {
        for (auto tex = it->second.begin(); tex != it->second.end(); tex++)
            delete *tex;
    }

    for (auto it = getTextures().begin(); it != getTextures().end(); it++)
        delete it->second;

    for (auto it = getMeshes().begin(); it != getMeshes().end(); it++)
        delete it->second;
}

GLint Renderer::getModelLoc() { return modelLoc; }
GLint Renderer::getColorLoc() { return colorLoc; }
json Renderer::getData() { return data; }
