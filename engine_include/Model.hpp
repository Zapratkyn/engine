#ifndef __MODEL__
	#define __MODEL__

class Model
{

public:

	Model();

	void move();
	void render();

private:

	GLuint sprite;
	GLuint shader;

};

#endif