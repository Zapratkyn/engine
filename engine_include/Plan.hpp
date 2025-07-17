#ifndef __AXIS__
	#define __AXIS__

class Axis
{

public:

	Plan(GLuint shader);

	void render();

private:

	GLuint axes[2];
	GLuint shader;

};

#endif