#include "../game_include/Cactus.hpp"

Cactus::Cactus(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 12.5f)
{
	// Use the randomizer to chose among the 6 different cactus models and set the width and height of the chosen model
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

	// They start on the right end of the screen, sligthly out of range, to appear already generated
	position = glm::vec3(1.05f, 0.0f, 0.0f);

	// The sprite
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos[type];

	// The hitbox as it appears on the screen (red rectangle)
	std::map<std::string, GLuint> hitboxes = loader->getHitboxes();
	hitbox = hitboxes[type];
}