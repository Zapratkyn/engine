#ifndef __PLAYER__
	#define __PLAYER__

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "../Graphics/Mesh.hpp"
#include "../Graphics/Helpers.h"

class Player
{

public:

	Player(const char *scene);
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