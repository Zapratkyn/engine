#ifndef __ANIMATION__
	#define __ANIMATION__

#include <string>
#include <vector>
#include <memory>
#include "Graphics/Mesh.hpp"

class Mesh;

struct Texture
{
    GLuint texture;
    int width, height;
};

struct FrameUV
{
    float u_min, v_min;
    float u_max, v_max;
};

struct Animation
{
    std::string name;
    std::shared_ptr<struct Texture> texture;
    std::vector<FrameUV> frames;
    int frameWidth, frameHeight;
    int frameCount; 
    float frameTime;
};

struct Terrain
{
    std::string name;
    GLuint texture;
};

struct Background
{
    std::shared_ptr<Texture> texture;
    float scaleX, scaleY;
};

struct Exit
{
    float x, y;
    std::string direction;
};

enum Direction
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};


#endif