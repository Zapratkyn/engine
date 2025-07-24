#include "Player.hpp"
#include "../Graphics/Renderer.hpp"
#include <nlohmann/json.hpp>
#include <thread>
#include <iostream>

using json = nlohmann::json;

Player::Player(const char *scene)
{
	auto anims = Renderer::getAnimations();
	animations["idle"] = anims["warrior_idle"];
	animations["guard"] = anims["warrior_guard"];
	animations["attack1"] = anims["warrior_attack1"];

	auto meshes = Renderer::getMeshes();
	mesh = meshes["unit"];

	json data = Renderer::getData();

	auto player = data["scenes"][scene]["player"];
	position = glm::vec2(player["x"], player["y"]);

	currentAnim = animations["idle"];
	currentFrame = 0;
}

void Player::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Player::StartAnimation() { lastUpdate = std::chrono::steady_clock::now(); }

void Player::Draw(GLint positionLoc)
{
	auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

    if (elapsed.count() >= currentAnim->frameTime) 
    {
        currentFrame = (currentFrame + 1) % currentAnim->frameCount;
        lastUpdate = now;

    	UpdateUVs();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);

    glBindTexture(GL_TEXTURE_2D, currentAnim->texture);
	glBindVertexArray(mesh->getVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// // Matrice de position
    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

    // // 1. Dessiner un quad coloré (même géométrie que le mesh, mais un peu plus grand)
    // glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    // glUniform4f(colorLoc, 0.1f, 0.1f, 0.1f, 1.0f); // fond sombre
    // glBindTexture(GL_TEXTURE_2D, 0);              // pas de texture
    // glBindVertexArray(mesh->getVAO());
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // // 2. Dessiner ensuite le mesh animé (avec texture)
    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.01f)); // léger décalage z
    // glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    // glBindTexture(GL_TEXTURE_2D, currentAnim->texture);
    // glBindVertexArray(mesh->getVAO());
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Player::UpdateUVs()
{
    float uSize = (float)currentAnim->frameWidth / currentAnim->textureWidth;
    float vSize = (float)currentAnim->frameHeight / currentAnim->textureHeight;

    float u = (currentFrame % (currentAnim->textureWidth / currentAnim->frameWidth)) * uSize;
    float v = 0.0f;

    std::vector<float> uvs = {
        u,         v,
        u + uSize, v,
        u + uSize, v + vSize,
        u,         v + vSize
    };

    mesh->UpdateUVs(uvs);
}

void Player::setGuarding(bool guard)
{
	guarding = guard;
	if (guard)
		currentAnim = animations["guard"];
	else
		currentAnim = animations["idle"];
	currentFrame = 0;
	StartAnimation();
}

bool Player::isGuarding() { return guarding; }
