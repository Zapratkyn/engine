#include "../game_include/Restart.hpp"

Restart::Restart(Loader *loader) : Object(loader)
{
	position = glm::vec3(-0.075f, 0.2f, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos["restart_button"];
}

void Restart::render(bool showHitbox)
{
	(void)showHitbox;
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Restart::update(float deltaTime) { (void)deltaTime; }