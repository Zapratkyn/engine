#ifndef __VISUALHELPER__
	#define __VISUALHELPER__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VisualHelper
{

public:

	VisualHelper(GLuint shader);
	virtual ~VisualHelper() = default;

	void toggleDisplay();
	bool isDisplayed();
	virtual void render() = 0;

protected:

	GLuint shader;
	GLuint lines[2];

	bool display;

	VisualHelper *vh;
};

#endif