#include "Scene.hpp"
#include "../Graphics/Renderer.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static Player *player;
// static std::vector<Unit*> units;

void Scene::Load(const char *scene)
{
	json data = Renderer::getData();
	if (!player)
		player = new Player(scene);
	else
	{
		auto pl = data["scenes"][scene]["player"];
		player->setPosition(pl["x"], pl["y"]);
	}
}

Player *Scene::getPlayer() { return player; }
// std::vector<Unit*> &Scene::getUnits() { return units; }