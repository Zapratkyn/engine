#include "../game_include/Player.hpp"

Player::Player(Loader *loader) : 
Object(loader)
{
	// Starting position, only modified on the Y axis when jumping
	position = glm::vec3(-0.75f, 0.0f, 0.0f);
	offset = glm::vec3(0.11f, 0.13f, 0.0f);

	hitboxPosition[0] = position;
	hitboxPosition[1] = position + offset;
	crawl = false;
	jumping = false;
	isDead = false;

	jump_velocity = 3.0f;
	speedY = 0.0f;
	gravity = 9.8f;

	frameCount = 0;

	// Hitboxes sizes, to detect collision
	hitboxWidths["standing_1"] = 0.13f;
	hitboxWidths["standing_2"] = 0.08f;
	hitboxWidths["crawling"] = 0.17f;
	hitboxHeights["standing_1"] = 0.1f;
	hitboxHeights["standing_2"] = 0.07f;
	hitboxHeights["crawling"] = 0.12f;

	// The sprites
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAOs["dino_standing_1"] = vaos["dino_standing_1"];
	VAOs["dino_standing_2"] = vaos["dino_standing_2"];
	VAOs["dino_crawling_1"] = vaos["dino_crawling_1"];
	VAOs["dino_crawling_2"] = vaos["dino_crawling_2"];
	VAOs["dino_dead"] = vaos["dino_dead"];

	// The hitboxes as displayed on the screen (blue rectangles)
	std::map<std::string, GLuint> loadedHitboxes = loader->getHitboxes();
	hitboxes["dino_standing_1"] = loadedHitboxes["dino_standing_1"];
	hitboxes["dino_standing_2"] = loadedHitboxes["dino_standing_2"];
	hitboxes["dino_crawling"] = loadedHitboxes["dino_crawling"];
}

void Player::render(bool showHitbox)
{
	// Sprite
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    // std::string 
    if (isDead)
    	glBindVertexArray(VAOs["dino_dead"]);
    else
    {
	    if (crawl)
			glBindVertexArray(VAOs[(frameCount % 40 > 20) ? "dino_crawling_1" : "dino_crawling_2"]); // Walk
		else
			glBindVertexArray(VAOs[(frameCount % 40 > 20 || jumping) ? "dino_standing_1" : "dino_standing_2"]); // Walk. It stops while jumping
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (showHitbox)
	{
		glUseProgram(hitboxShader);
		glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
		if (crawl)
		{
			glBindVertexArray(hitboxes["dino_crawling"]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else
		{
			// Bind the rectangle for coming drawings
			glBindVertexArray(hitboxes["dino_standing_1"]);

			// Draw the lower rectangle, his bottom-left corner being the same as the dino sprite's, no new translate() needed
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Translate to the second rectangle's position, relative to the dino sprite
			glBindVertexArray(hitboxes["dino_standing_2"]);
			model = glm::translate(model, offset);
			glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);

			// Draw the upper rectangle
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
    }

    // An int used to create the walking effect
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

void Player::update(std::list<Object*> &enemies, float deltaTime, bool &dead)
{
	if (jumping)
	{
		// Jumping physics for the dino and his hitboxes
		speedY -= gravity * deltaTime;
		glm::vec3 momentum = glm::vec3(0.0f, speedY, 0.0f);
		position += momentum * deltaTime;
		hitboxPosition[0] += momentum * deltaTime;
		hitboxPosition[1] += momentum * deltaTime;

		// When the dino touches the ground, it stops jumping
		if (position.y <= 0.0f)
		{
			position = glm::vec3(-0.75f, 0.0f, 0.0f);
			hitboxPosition[0] = position;
			hitboxPosition[1] = position + offset;
			speedY = 0.0f;
			jumping = false;
		}
	}

	// Finding the upper right corner of the lower hitbox (while standing) or the only hitbox (while crawling)
	glm::vec3 max1 = glm::vec3(hitboxPosition[0].x + (crawl ? hitboxWidths["crawling"] : hitboxWidths["standing_1"]), hitboxPosition[0].y + (crawl ? hitboxHeights["crawling"] : hitboxHeights["standing_1"]), 0.0f);

	// Finding the upper right corner of the upper hitbox (always, only used if standing)
	glm::vec3 max2 = glm::vec3(hitboxPosition[1].x + hitboxWidths["standing_2"], hitboxPosition[1].y + hitboxHeights["standing_2"], 0.0f);

	// Bottom left corners are already known as hitboxPosition[0] and hitboxPosition[1]

	for (auto it = enemies.begin();it != enemies.end();)
	{
		// Find bottom left and upper right corner of the enemy's hitbox
		glm::vec3 enemy_min = (*it)->getMin();
		glm::vec3 enemy_max = (*it)->getMax();

		// Check if the lower hitbox (while standing) or the only hitbox (while crawling) hits the enemy
		if ((hitboxPosition[0].x <= enemy_max.x && max1.x >= enemy_min.x) && (hitboxPosition[0].y <= enemy_max.y && max1.y >= enemy_min.y))
		{
			isDead = true;
			dead = true;
			break;
		}
		// If standing, check if the upper hitbox hits the enemy
		if (!crawl && (hitboxPosition[1].x <= enemy_max.x && max2.x >= enemy_min.x) && (hitboxPosition[1].y <= enemy_max.y && max2.y >= enemy_min.y))
		{
			isDead = true;
			dead = true;
			break;
		}

		// Since enemies are discarded when they reach the left side of the screen, we only check existing enemies, i.e. the ones on the screen

       it++;
	}
}

void Player::restart()
{
	// Reset sprite and hitboxes' positions
	position = glm::vec3(-0.75f, 0.0f, 0.0f);
	hitboxPosition[0] = position;
	hitboxPosition[1] = position + offset;
	
	crawl = false;
	jumping = false;
	isDead = false;
	speedY = 0.0f;
	frameCount = 0;
}
