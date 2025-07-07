#include "../game_include/Cloud.hpp"

Cloud::Cloud(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 2.5f)
{
	// Use randomizer to determine altitude, anywhere between right above the ground to almost touching the upper side of the screen
	int random = randomizer(motor);
	float height = (float)random / 100;
	if (height < 0.1f)
		height = 0.1f;
	position = glm::vec3(1.05f, height, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos["cloud"];
}

void Cloud::render(bool showHitbox)
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

void Cloud::update(float deltaTime)
{
	// Use the speed to make them move from right to left
	float velocity = speed * deltaTime;
	position += movement * velocity;
}