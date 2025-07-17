#ifndef __GRID__
	#define __GRID__

#include "VisualHelper.hpp"
#include "Plan.hpp"

class VisualHelper;
class Plan;

class Grid : public VisualHelper
{

public:

	Grid(GLuint shader, Plan *plan);

	void render();

private:

	GLint positionLoc;
};

#endif