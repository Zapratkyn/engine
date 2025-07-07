#include "../game_include/Cactus.hpp"

Cactus::Cactus(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 12.5f)
{
	int random = randomizer(motor);
	std::string type;
	if (random < 16)
	{
		type = "cactus_small_1";
		width = 0.05f;
	}
	else if (random < 32)
	{
		type = "cactus_small_2";
		width = 0.1f;
	}
	else if (random < 48)
	{
		type = "cactus_small_3";
		width = 0.15f;
	}
	else if (random < 59)
	{
		type = "cactus_tall_1";
		width = 0.1f;
	}
	else if (random < 80)
	{
		type = "cactus_tall_2";
		width = 0.2f;
	}
	else
	{
		type = "cactus_mix";
		width = 0.3f;
	}
	if (random < 48)
		height = 0.1f;
	else
		height = 0.2f;
	position = glm::vec3(1.05f, 0.0f, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos[type];
	std::map<std::string, GLuint> hitboxes = loader->getHitboxes();
	hitbox = hitboxes[type];
}

void Cactus::render(bool showHitbox)
{
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (showHitbox)
	{
		glUseProgram(hitboxShader);
	    glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
	   	glBindVertexArray(hitbox);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void Cactus::update(float deltaTime)
{
	float velocity = speed * deltaTime;
	position += movement * velocity;
}