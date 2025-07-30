#include "Scene.hpp"
#include "../Graphics/Renderer.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string &Scene::getSceneName()
{
	static std::string sceneName;
	return sceneName;
}

static Player *player;
// static std::vector<Unit*> units;

void Scene::Load(const char *scene, std::string &unit)
{
	json data = Renderer::getData();
	if (!player)
		player = new Player(scene, unit);
	else
	{
		auto pl = data["scenes"][scene]["player"];
		player->setPosition(pl["x"], pl["y"]);
	}

	getSceneName() = scene;
}

void Scene::Update()
{
	player->Update();
}

Player *Scene::getPlayer() { return player; }
// std::vector<Unit*> &Scene::getUnits() { return units; }