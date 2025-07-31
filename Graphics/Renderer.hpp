#ifndef __RENDERER__
	#define __RENDERER__

#include <unordered_map>
#include <glad/glad.h>
#include <nlohmann/json.hpp>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Helpers.h"

using json = nlohmann::json;

namespace Renderer
{
	void Init();
	void Draw();
    void Free();

    std::unordered_map<std::string, Shader*> &getShaders();
    std::unordered_map<std::string, struct Texture*> &getTextures();
    std::unordered_map<std::string, std::unordered_map<std::string, struct Animation*>> &getAnimations();
    std::unordered_map<std::string, Mesh*> &getMeshes();


    GLint getModelLoc();
    GLint getColorLoc();
    json getData();

}

#endif