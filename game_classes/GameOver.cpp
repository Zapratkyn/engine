#include "../game_include/GameOver.hpp"

GameOver::GameOver(Loader *loader) : Object(loader)
{
	position = glm::vec3(-0.5f, 0.5f, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos["game_over"];
}