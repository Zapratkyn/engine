#include "../game_include/data.hpp"

void getSceneData(Scene scene, std::map<std::string, std::vector<int>> *coords, std::map<std::string, std::vector<float>> *positions)
{
	switch (scene)
	{
		case DINO_GAME:
			*coords = {
				{"dino_standing_1", {936, 2, 44, 47}},
				{"dino_standing_2", {980, 2, 44, 47}},
				{"dino_crawling_1", {1112, 2, 59, 47}},
				{"dino_crawling_2", {1171, 2, 59, 47}},
				{"bird_1", {134, 2, 46, 40}},
				{"bird_2", {180, 2, 46, 40}}
			};
			*positions = {
				{"dino_standing_1", {0.20f, 0.20f, 0.20f, 0.20f}},
				{"dino_standing_2", {0.20f, 0.20f, 0.20f, 0.20f}},
				{"dino_crawling_1", {0.20f, 0.20f, 0.20f, 0.20f}},
				{"dino_crawling_2", {0.20f, 0.20f, 0.20f, 0.20f}},
				{"bird_1", {0.15f, 0.15f, 0.15f, 0.15f}},
				{"bird_2", {0.15f, 0.15f, 0.15f, 0.15f}}
			};
	}
}

std::string getAssetsPath(Scene scene)
{
	switch (scene)
	{
		case DINO_GAME:
			return "assets/100-offline-sprite.png";
	}

	return "";
}