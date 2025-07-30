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

void Scene::Load(const char *scene)
{
	if (!player)
		player = new Player(scene);
	else
	{
		json data = Renderer::getData();
		auto pl = data["scenes"][scene]["player"]["position"];
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