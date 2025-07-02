#include "../game_include/Cloud.hpp"

Cloud::Cloud(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 2.5f)
{
	int random = randomizer(motor);
	float height = random >= 50 ? 0.4f : 0.6f;
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

void Cloud::move(float deltaTime)
{
	float velocity = speed * deltaTime;
	position += movement * velocity;
}

float Cloud::getPosition() { return position.x; }