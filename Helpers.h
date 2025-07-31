#ifndef __ANIMATION__
	#define __ANIMATION__

#include <string>
#include <vector>
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
    struct Texture *texture;
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
    struct Texture *texture;
    float scaleX, scaleY;
};

enum Direction
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

#endif