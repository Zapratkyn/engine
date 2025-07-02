#include "../game_include/Landscape.hpp"

Landscape::Landscape(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 5.0f)
{
	position = glm::vec3(1.05f, 0.0f, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	int random = randomizer(motor);
	if (random <= 33)
		VAO = vaos["landscape_1"];
	else if (random <= 66)
		VAO = vaos["landscape_2"];
	else
		VAO = vaos["landscape_3"];
}

void Landscape::render(bool showHitbox)
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

void Landscape::move(float deltaTime)
{
	float velocity = speed * deltaTime;
	position += movement * velocity;
}

float Landscape::getPosition() { return position.x; }