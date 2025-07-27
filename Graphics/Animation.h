#ifndef __ANIMATION__
	#define __ANIMATION__

#include <string>

struct Animation
{
    std::string name;
    GLuint texture;
    int frameWidth, frameHeight;
    int textureWidth, textureHeight;
    int frameCount, framesPerRow;
    float frameTime;
};

enum Direction
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

#endif