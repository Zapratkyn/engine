#include "../game_include/Cloud.hpp"

Cloud::Cloud(Loader *loader, std::uniform_int_distribution<int> &randomizer, std::mt19937 &motor) : 
Object(loader, glm::vec3(-0.1f, 0.0f, 0.0f), 2.5f)
{
	// Use randomizer to determine altitude, anywhere between right above the ground to almost touching the upper side of the screen
	int random = randomizer(motor);
	float height = (float)random / 100;
	if (height < 0.1f)
		height = 0.1f;

	// They start on the right end of the screen, sligthly out of range, to appear already generated
	position = glm::vec3(1.05f, height, 0.0f);

	// The sprite
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos["cloud"];

	// No hitbox since it is not an enemy
}