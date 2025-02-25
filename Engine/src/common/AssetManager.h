#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include<map>
#include"game_objects/Model.h"
#include"game_objects/animation/Animator.h"
#include"SkyBox.h"

namespace AssetManager {
	void load();
	Model* get_model(uint8_t id);
	SkyBox* get_sky_box();
	uint8_t get_card(uint8_t card);
	unsigned int get_grass();
};
#endif // !ASSET_MANAGER_H