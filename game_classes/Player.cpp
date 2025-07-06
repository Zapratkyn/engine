#include "../game_include/Player.hpp"

Player::Player(Loader *loader) : 
Object(loader)
{
	position = glm::vec3(-0.75f, 0.0f, 0.0f);
	crawl = false;
	jumping = false;
	jump_velocity = 3.0f;
	speedY = 0.0f;
	gravity = 9.8f;
	frameCount = 0;
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAOs[0] = vaos["dino_standing_1"];
	VAOs[1] = vaos["dino_standing_2"];
	VAOs[2] = vaos["dino_crawling_1"];
	VAOs[3] = vaos["dino_crawling_2"];
	VAOs[4] = vaos["dino_dead"];
	std::map<std::string, GLuint> hitboxes = loader->getHitboxes();
	hitbox[0] = hitboxes["dino_standing"];
	hitbox[1] = hitboxes["dino_crawling"];
}

void Player::render(bool showHitbox)
{
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    if (crawl)
		glBindVertexArray(VAOs[(frameCount % 40 > 20) ? 2 : 3]);
	else
		glBindVertexArray(VAOs[(frameCount % 40 > 20) ? 0 : 1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(hitboxShader);
    glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
   	glBindVertexArray(hitbox[crawl ? 1 : 0]);
	if (showHitbox)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	frameCount++;
	if (frameCount >= 10000)
		frameCount = 0;
}

void Player::move(direction direction)
{
	if (direction == CROUCH && !crawl)
		crawl = true;
	else if (direction == STAND && crawl)
		crawl = false;
	else if (direction == JUMP && !jumping)
	{
		speedY = jump_velocity;
		jumping = true;
	}
}

void Player::update(float deltaTime)
{
	if (jumping)
	{
		speedY -= gravity * deltaTime;
		position += glm::vec3(0.0f, speedY, 0.0f) * deltaTime;

		if (position.y <= 0.0f)
		{
			position = glm::vec3(-0.75f, 0.0f, 0.0f);
			speedY = 0.0f;
			jumping = false;
		}
	}
}
