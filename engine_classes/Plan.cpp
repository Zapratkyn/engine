#include "../engine_include/Plan.hpp"

Plan::Plan(GLuint shader) : VisualHelper(shader) {}

void Plan::render()
{
    if (!vh->isDisplayed())
    {
        glUseProgram(shader);
        glBindVertexArray(lines[0]);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(lines[1]);
        glDrawArrays(GL_LINES, 0, 2);
    }
}

void Plan::setGrid(Grid *grid) { vh = grid; }