#include "Player.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Core/Config.hpp"
#include <nlohmann/json.hpp>
#include <thread>
#include <iostream>

using json = nlohmann::json;

Player::Player(const char *scene)
{
	json data = Renderer::getData();

	auto anims = Renderer::getAnimations()[data["scenes"][scene]["player"]["unit"]];
	for (auto it = anims.begin(); it != anims.end(); it++)
		animations[it->first] = it->second;

	auto meshes = Renderer::getMeshes();
	mesh = meshes["unit"];

	auto player = data["scenes"][scene]["player"]["position"];
	position = glm::vec2(player["x"], player["y"]);

	currentAnim = animations["idle"];
	currentFrame = 0;

	// facing = "right";
	reverseX = false;
	guarding = false;
	attacking = false;
	moving = false;
	displayMesh = false;
	type = data["scenes"][scene]["player"]["unit"];

	for (int i = 0; i < 4; i++)
		directions[i] = false;
}

void Player::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Player::StartAnimation() 
{ 
	currentFrame = 0;
	lastUpdate = std::chrono::steady_clock::now(); 
	mesh->UpdateUVs(currentAnim->frames[currentFrame]);
}

void Player::Draw()
{
	auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

    if (elapsed.count() >= currentAnim->frameTime) 
    {
        currentFrame = (currentFrame + 1) % currentAnim->frameCount;
        lastUpdate = now;

    	mesh->UpdateUVs(currentAnim->frames[currentFrame]);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    glm::mat4 model = glm::mat4(1.0f);
    std::unordered_map config = getConfig();
    float px = position.x * config["width"];
	float py = position.y * config["height"] + currentAnim->frameHeight / 2.0f;
	model = glm::translate(model, glm::vec3(px, py, 0.0f));
	model = glm::scale(model, glm::vec3(reverseX ? -currentAnim->frameWidth : currentAnim->frameWidth, currentAnim->frameHeight, 1.0f));
	if (type == "knight")
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
	glUniformMatrix4fv(Renderer::getModelLoc(), 1, GL_FALSE, &model[0][0]);

    if (!displayMesh)
	{
	    glBindTexture(GL_TEXTURE_2D, currentAnim->texture->texture);
		glBindVertexArray(mesh->getVAO());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	else
	{

	   	GLint modelLoc = Renderer::getModelLoc();
	   	GLint colorLoc = Renderer::getColorLoc();
	    // 1. Dessiner un quad coloré (même géométrie que le mesh, mais un peu plus grand)
	    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	    glUniform4f(colorLoc, 0.1f, 0.1f, 0.1f, 1.0f); // fond sombre
	    glBindTexture(GL_TEXTURE_2D, 0);              // pas de texture
	    glBindVertexArray(mesh->getVAO());
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	    // 2. Dessiner ensuite le mesh animé (avec texture)
	    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.01f)); // léger décalage z
	    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	    glBindTexture(GL_TEXTURE_2D, currentAnim->texture->texture);
	    glBindVertexArray(mesh->getVAO());
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void Player::Update()
{
	if (guarding)
		return;
	if (moving && directions[RIGHT])
		position.x += 0.005;
	if (moving && directions[LEFT])
		position.x -= 0.005;
	if (moving && directions[DOWN])
		position.y -= 0.005;
	if (moving && directions[UP])
		position.y += 0.005;
	if ((currentAnim->name == "attack1" || currentAnim->name == "attack2") && currentFrame == 3)
	{
		attacking = false;
		currentAnim = animations[moving ? "run" : "idle"];
		StartAnimation();
	}
}

void Player::setGuarding(bool guard)
{
	guarding = guard;
	if (guard)
		currentAnim = animations["guard"];
	else if (moving)
		currentAnim = animations["run"];
	else
		currentAnim = animations["idle"];
	StartAnimation();
}

void Player::Attack(std::string attack)
{
	if (guarding)
		return;
	attacking = true;
	currentAnim = animations["attack" + attack];
	StartAnimation();
}

void Player::setMoving(Direction direction, bool move)
{
	directions[direction] = move;

	if (move && !moving)
		moving = true;

	if (!guarding && !attacking)
		currentAnim = animations["run"];
	if ((reverseX && direction == RIGHT && !directions[LEFT]) || (!reverseX && direction == LEFT && !directions[RIGHT]))
		reverseX = !reverseX;

	// Trying to move in two opposite directions at once will stop the move
	if (move && ((directions[LEFT] && directions[RIGHT]) || (directions[DOWN] && directions[UP])))
	{
		moving = false;
		if (!guarding)
			currentAnim = animations["idle"];
	}

	// Releasing all the keys will stop the move
	if (!move && !directions[LEFT] && !directions[RIGHT] && !directions[DOWN] && !directions[UP])
	{
		moving = false;
		if (!guarding)
			currentAnim = animations["idle"];
	}

	// While pressing two opposite directions buttons, releasing one will make the character move in (and face in the case of left or right) the other
	if (!move && ((direction == LEFT && directions[RIGHT]) || (direction == RIGHT && directions[LEFT]) || (direction == UP && directions[DOWN]) || (direction == DOWN && directions[UP])))
	{
		moving = true;
		reverseX = direction == LEFT ? false : true;
		if (!guarding)
			currentAnim = animations["run"];
	}

	/* 
	TODO : When pressing 3 directions at once, releasing the one not opposite to another should not trigger the running animation
	E.g. : I press left, right and up. Idle is triggered. If I release up, it should not trigger running while not moving
	*/ 

	StartAnimation();
}

bool Player::isGuarding() { return guarding; }
bool Player::isMoving() { return moving; }
void Player::ToggleMeshDisplay() { displayMesh = !displayMesh; }
std::string Player::getType() { return type; }
