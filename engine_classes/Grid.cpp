#include "../engine_include/Grid.hpp"

Grid::Grid(GLuint shader, Plan *plan) : VisualHelper(shader)
{
	vh = plan;
	positionLoc = glGetUniformLocation(shader, "position");
}

void Grid::render()
{
	if (!vh->isDisplayed())
	{
        glUseProgram(shader);
        glBindVertexArray(lines[0]);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
        for (int i = 0; i < 10; i++)
        {
			model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
			glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_LINES, 0, 2);
		}
		glBindVertexArray(lines[1]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
        for (int i = 0; i < 10; i++)
        {
			model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
			glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
}