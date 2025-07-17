#ifndef __PLAN__
	#define __PLAN__

#include "VisualHelper.hpp"
#include "Grid.hpp"

class Grid;

class Plan : public VisualHelper
{

public:

	Plan(GLuint shader);

	void render();
	void setGrid(Grid *grid);

};

#endif