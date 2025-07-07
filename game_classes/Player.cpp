#include "../game_include/Player.hpp"

Player::Player(Loader *loader) : 
Object(loader)
{
	position = glm::vec3(-0.75f, 0.0f, 0.0f);
	hitboxPosition[0] = position;
	hitboxPosition[1] = position + glm::vec3(0.05f, 0.1f, 0.0f);
	crawl = false;
	jumping = false;
	dead = false;
	jump_velocity = 3.0f;
	speedY = 0.0f;
	gravity = 9.8f;
	frameCount = 0;
	width = 0.15f;
	height = 0.1f;
	height_crawling = 0.12f;
	width_crawling = 0.2f;
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
    if (dead)
    	glBindVertexArray(VAOs[4]);
    else
    {
	    if (crawl)
			glBindVertexArray(VAOs[(frameCount % 40 > 20) ? 2 : 3]);
		else
			glBindVertexArray(VAOs[(frameCount % 40 > 20 || jumping) ? 0 : 1]);
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (showHitbox)
	{
		glUseProgram(hitboxShader);
		glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
		if (crawl)
		{
			glBindVertexArray(hitbox[1]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glBindVertexArray(hitbox[0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			model = glm::translate(model, glm::vec3(0.05f, 0.1f, 0.0f));
			glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
    }
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
	else if (direction == JUMP && !jumping && !crawl)
	{
		speedY = jump_velocity;
		jumping = true;
	}
}

void Player::update(float deltaTime) { (void)deltaTime; }

void Player::update(std::list<Object*> &enemies, float deltaTime)
{
	if (jumping)
	{
		speedY -= gravity * deltaTime;
		glm::vec3 momentum = glm::vec3(0.0f, speedY, 0.0f);
		position += momentum * deltaTime;
		hitboxPosition[0] += momentum * deltaTime;
		hitboxPosition[1] += momentum * deltaTime;

		if (position.y <= 0.0f)
		{
			position = glm::vec3(-0.75f, 0.0f, 0.0f);
			hitboxPosition[0] = position;
			hitboxPosition[1] = position + glm::vec3(0.05f, 0.1f, 0.0f);
			speedY = 0.0f;
			jumping = false;
		}
	}

	glm::vec3 max1 = glm::vec3(hitboxPosition[0].x + (crawl ? width_crawling : width), hitboxPosition[0].y + (crawl ? height_crawling : height), 0.0f);
	glm::vec3 max2 = glm::vec3(hitboxPosition[1].x + width, hitboxPosition[1].y + height, 0.0f);

	for (auto it = enemies.begin();it != enemies.end();)
	{
		glm::vec3 enemy_min = (*it)->getMin();
		glm::vec3 enemy_max = (*it)->getMax();

		if ((hitboxPosition[0].x <= enemy_max.x && max1.x >= enemy_min.x) && (hitboxPosition[0].y <= enemy_max.y && max1.y >= enemy_min.y))
		{
			dead = true;
			break;
		}
		if (!crawl && (hitboxPosition[1].x <= enemy_max.x && max2.x >= enemy_min.x) && (hitboxPosition[1].y <= enemy_max.y && max2.y >= enemy_min.y))
		{
			dead = true;
			break;
		}

       it++;
	}
}

bool Player::getDead() const { return dead; }
