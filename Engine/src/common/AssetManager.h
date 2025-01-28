#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include<map>
#include"game_objects/Model.h"
#include"game_objects/animation/Animator.h"
#include"SkyBox.h"

namespace AssetManager {
	void load();
	Model* get_model(unsigned int id);
	SkyBox* get_sky_box();
	unsigned int get_card(unsigned int);
};
#endif // !ASSET_MANAGER_H