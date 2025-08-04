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
std::unordered_map<std::string, std::vector<struct Background*>> &Renderer::getDecors()
{
    static std::unordered_map<std::string, std::vector<struct Background*>> decors;
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
static GLint modelLoc, colorLoc, projectionLoc, scaleLoc;

// Parsed data.json
static json data;

// Projection matrix for the units drawing
glm::mat4 projection;


struct Texture *ParseTexture(std::string texName, std::string texSource, std::string type);
struct Animation *ParseAnim(std::string animName, json::iterator::reference &animData, struct Texture *texture);

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
    // scrollOffsetLoc = glGetUniformLocation(getShaders()["background"]->ID, "scrollOffset");
    // spacingFactorLoc = glGetUniformLocation(getShaders()["background"]->ID, "spacingFactor");
    // patternWidthLoc = glGetUniformLocation(getShaders()["background"]->ID, "patternWidth");

    auto config = getConfig();
    projection = glm::ortho(0.0f, (float)config["width"], 0.0f, (float)config["height"]);

    getMeshes()["unit"] = new Mesh;
    getMeshes()["background"] = new Mesh;

    float array1[] = { -0.5f, -0.5f, 0.5f, -0.5, 0.5f, 0.5f, -0.5f, 0.5f };
    getMeshes()["unit"]->SetArray(array1, sizeof(array1), 0);
    // float array2[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    // getMeshes()["unit"]->SetArray(array2, sizeof(array2), 1);
    // float array3[] = { -50.0f, -0.8f, 50.0f, -0.8f, 50.0f, 0.8f, -50.0f, 0.8f };
    float array3[] = { -1.0f, -0.8f, 1.0f, -0.8f, 1.0f, 0.8f, -1.0f, 0.8f };
    getMeshes()["background"]->SetArray(array3, sizeof(array3), 0);
    // float array4[] = { 0.0f, 0.0f, 10.0f, 0.0f, 10.0f, 1.0f, 0.0f, 1.0f };
    // float array4[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    // getMeshes()["background"]->SetArray(array4, sizeof(array4), 1);
}

void Renderer::ParseScene(const char *scene)
{
    // We load only the textures we need for the scene

    auto sceneToLoad = data["scenes"][scene];
    std::vector<std::string> unitsToLoad{sceneToLoad["player"]["unit"]};

    for (auto& [unitName, unitData] : sceneToLoad["units"].items())
    {
        if (std::find(unitsToLoad.begin(), unitsToLoad.end(), unitName) == unitsToLoad.end())
            unitsToLoad.push_back(unitName);
    }

    for (auto& unitName : unitsToLoad)
    {
        if (getAnimations().find(unitName) != getAnimations().end())
            continue;
        std::unordered_map<std::string, struct Texture*> associations;
        for (auto& [animName, animData] : data["animations"][unitName].items())
        {
            std::string texName(animData["texture"]);
            auto it = associations.find(texName);
            if (it != associations.end())
                getAnimations()[unitName][animName] = ParseAnim(animName, animData, it->second);
            else
            {
                struct Texture *newTex = ParseTexture(texName, data["animations_textures"][texName], "animation");
                getTextures()[texName] = newTex; // Keeping texture for future scenes
                associations[texName] = newTex;
                getAnimations()[unitName][animName] = ParseAnim(animName, animData, newTex);
            }
        }
    }

    for (auto& [bgName, bgData] : data["decors"][sceneToLoad["decor"]]["back"].items())
    {
        struct Background *bg = new Background;
        bg->scaleX = bgData["x"];
        bg->scaleY = bgData["y"];
        std::string texName(bgName);
        bg->texture = ParseTexture(texName, data["decors_textures"][texName], "decor");
        getDecors()["back"].push_back(bg);
    }

    // for (auto& bgName : data["decors"][sceneToLoad["decor"]]["front"])
    // {
    //     std::string texName(bgName);
    //     getDecors()["front"].push_back(ParseTexture(texName, data["decors_textures"][texName], "decor"));
    // }
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

struct Animation *ParseAnim(std::string animName, json::iterator::reference &animData, struct Texture *texture)
{
    struct Animation *anim = new struct Animation;

    anim->name = animName;
    anim->texture = texture;
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

    // Black bands above and below the image
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing decor
    getShaders()["background"]->use();
    glBindVertexArray(getMeshes()["background"]->getVAO());
    auto bgs = getDecors()["back"];
    for (size_t i = 0; i < bgs.size(); i++)
    {
        auto bg = bgs[i];
        glUniform2f(scaleLoc, bg->scaleX, bg->scaleY);
        glBindTexture(GL_TEXTURE_2D, bg->texture->texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // Drawing units
    getShaders()["unit"]->use();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    Scene::getPlayer()->Draw();
}

// Free current scene, followed by parsing a new one
// TODO : Prevent deletion of assets needed bby the new scene
// void Renderer::Free(const char *newScene)
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
        for (auto bg : it->second)
        {
            delete bg->texture;
            delete bg;
        }
    }

    for (auto it = getTextures().begin(); it != getTextures().end(); it++)
        delete it->second;

    for (auto it = getMeshes().begin(); it != getMeshes().end(); it++)
        delete it->second;
}

GLint Renderer::getModelLoc() { return modelLoc; }
GLint Renderer::getColorLoc() { return colorLoc; }
json Renderer::getData() { return data; }
