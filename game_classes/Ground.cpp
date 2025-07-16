#include "../game_include/Ground.hpp"

Ground::Ground(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 12.5f)
{
	// It starts on the right end of the screen, sligthly out of range, to appear already generated
	position = glm::vec3(1.05f, 0.0f, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();

	// Use randomizer to chose between 3 sprites
	int random = randomizer(motor);
	if (random <= 33)
		VAO = vaos["landscape_1"];
	else if (random <= 66)
		VAO = vaos["landscape_2"];
	else
		VAO = vaos["landscape_3"];

	// No hitbox since it is not an enemy
}