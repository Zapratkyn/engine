#include "../game_include/Restart.hpp"

Restart::Restart(Loader *loader) : Object(loader)
{
	position = glm::vec3(-0.1f, 0.12f, 0.0f);
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAO = vaos["restart_button"];
}