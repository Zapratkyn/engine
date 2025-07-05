#include "../game_include/data.hpp"

void getSceneData(Scene scene, std::map<std::string, std::vector<int>> *coords, std::map<std::string, std::vector<float>> *positions, std::map<std::string, std::vector<float>> *hitboxes)
{
	switch (scene)
	{
		case DINO_GAME:
			*coords = {
				{"dino_standing_1", {938, 4, 40, 43}},
				{"dino_standing_2", {982, 4, 40, 43}},
				{"dino_crawling_1", {1114, 4, 55, 43}},
				{"dino_crawling_2", {1173, 4, 55, 43}},
				{"bird_1", {134, 2, 46, 40}},
				{"bird_2", {180, 2, 46, 40}},
				{"cloud", {86, 2, 46, 13}},
				{"landscape_1", {1, 54, 400, 14}},
				{"landscape_2", {401, 54, 400, 14}},
				{"landscape_3", {801, 54, 400, 14}},
				{"cactus_small_1", {229, 3, 15, 33}},
				{"cactus_small_2", {246, 3, 32, 33}},
				{"cactus_small_3", {280, 3, 49, 33}},
				{"cactus_tall_1", {333, 3, 23, 49}},
				{"cactus_tall_2", {358, 3, 48, 49}},
				{"cactus_mix", {408, 3, 73, 49}}
			};
			*positions = {
				{"dino_standing_1", {0.20f, 0.20f}},
				{"dino_standing_2", {0.20f, 0.20f}},
				{"dino_crawling_1", {0.20f, 0.20f}},
				{"dino_crawling_2", {0.20f, 0.20f}},
				{"bird_1", {0.15f, 0.15f}},
				{"bird_2", {0.15f, 0.15f}},
				{"cloud", {0.15f, 0.05f}},
				{"landscape_1", {0.66f, 0.05f}},
				{"landscape_2", {0.66f, 0.05f}},
				{"landscape_3", {0.66f, 0.05f}},
				{"cactus_small_1", {0.05f, 0.1f}},
				{"cactus_small_2", {0.1f, 0.1f}},
				{"cactus_small_3", {0.15f, 0.1f}},
				{"cactus_tall_1", {0.1f, 0.2f}},
				{"cactus_tall_2", {0.2f, 0.2f}},
				{"cactus_mix", {0.3f, 0.2f}}
			};
			*hitboxes = {
				{"dino_standing", {0.2f, 0.2f, 0.0f, 0.0f, 1.0f}},
				{"dino_crawling", {0.2f, 0.12f, 0.0f, 0.0f, 1.0f}},
				{"bird", {0.15f, 0.15f, 1.0f, 0.0f, 0.0f}},
				{"cactus_small_1", {0.05f, 0.1f, 1.0f, 0.0f, 0.0f}},
				{"cactus_small_2", {0.1f, 0.1f, 1.0f, 0.0f, 0.0f}},
				{"cactus_small_3", {0.15f, 0.1f, 1.0f, 0.0f, 0.0f}},
				{"cactus_tall_1", {0.1, 0.2f, 1.0f, 0.0f, 0.0f}},
				{"cactus_tall_2", {0.2f, 0.2f, 1.0f, 0.0f, 0.0f}},
				{"cactus_mix", {0.3f, 0.2f, 1.0f, 0.0f, 0.0f}}
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