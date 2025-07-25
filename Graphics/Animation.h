#ifndef __ANIMATION__
	#define __ANIMATION__

struct Animation
{
    GLuint texture;
    int frameWidth, frameHeight;
    int textureWidth, textureHeight;
    int frameCount, framesPerRow;
    float frameTime;
};

#endif