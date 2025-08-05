#include "Scene.hpp"
#include "../Graphics/Renderer.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

std::string &Scene::getSceneName()
{
	static std::string sceneName;
	return sceneName;
}

std::unordered_map<std::string, struct Exit> &Scene::getExits()
{
	static std::unordered_map<std::string, struct Exit> exits;
	return exits;
}

// std::vector<Unit*> &Scene::getUnits()
// {
// 	static std::vector<Unit*> units;
// 	return units;
// }

static Player *player;

void Scene::Load(std::string scene)
{
	Renderer::Free(scene);
	Renderer::ParseScene(scene);
	json data = Renderer::getData();
	if (!player)
		player = new Player();

	getExits().clear();

	for (auto& [name, data] : data["scenes"][scene]["exits"].items())
	{
		struct Exit newExit;
		newExit.x = data["x"];
		newExit.y = data["y"];
		newExit.direction = data["direction"];
		getExits()[name] = newExit;
	}

	getSceneName() = scene;
}

void Scene::Update()
{
	player->Update();
}

void Scene::ShutDown()
{
	delete player;
	// for (auto it = units.begin(); it != units.end(); it++)
	// 	delete *it;
}

Player *Scene::getPlayer() { return player; }
// std::vector<Unit*> &Scene::getUnits() { return units; }