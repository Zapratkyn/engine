#ifndef __DATA__
	#define __DATA__

#include <map>
#include <vector>
#include <string>

enum Scene
{
	DINO_GAME
};

void getSceneData(Scene scene, std::map<std::string, std::vector<int>> *coords, std::map<std::string, std::vector<float>> *positions);
std::string getAssetsPath(Scene scene);

#endif