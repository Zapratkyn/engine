#include "../game_include/Bird.hpp"

Bird::Bird(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 10.5f)
{
	// Use randomizer to determine the bird's altitude (between 2 options, one forcing the dino to crouch)
	// It starts on the right end of the screen, sligthly out of range, to appear already generated
	int random = randomizer(motor);
	position = glm::vec3(1.05f, (random >= 50 ? 0.13f : 0.25f), 0.0f);

	// This is used to switch between the 2 sprites to make the bird look like it flies
	frameCount = 0;

	width = 0.15f;
	height = 0.15f;

	// The sprites
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAOs[0] = vaos["bird_1"];
	VAOs[1] = vaos["bird_2"];

	// The hitbox as it appears on the screen (red rectangle)
	std::map<std::string, GLuint> hitboxes = loader->getHitboxes();
	hitbox = hitboxes["bird"];
}

void Bird::render(bool showHitbox)
{
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAOs[(frameCount % 40 > 20) ? 0 : 1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (showHitbox)
	{
		glUseProgram(hitboxShader);
	    glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
	   	glBindVertexArray(hitbox);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	frameCount++;
	if (frameCount >= 10000)
		frameCount = 0;
}