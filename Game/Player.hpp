#ifndef __PLAYER__
	#define __PLAYER__

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <nlohmann/json.hpp>
#include "../Graphics/Mesh.hpp"
#include "../Helpers.h"

using json = nlohmann::json;

class Player
{

public:

	Player();
	void setPosition(float x, float y);
	void Update();
	void Draw();
	void setGuarding(bool guard);
	bool isGuarding();
	void setMoving(Direction direction, bool move);
	bool isMoving();
	void ToggleMeshDisplay();
	void Attack(std::string attack);
	std::string getType();
	json getData();

private:

	std::string type;

	std::unordered_map<std::string, struct Animation*> animations;
	struct Animation *currentAnim;
	int currentFrame;
	std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

	glm::vec2 position;

	Mesh *mesh;

	bool guarding;
	bool attacking;

	bool directions[4];
	bool moving;
	bool reverseX;

	bool displayMesh;

	void StartAnimation();
};

#endif