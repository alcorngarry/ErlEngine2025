#include"AssetManager.h"

std::map<uint8_t, Model*> assetMap;
std::map<uint16_t, unsigned int> uiAssetMap;
SkyBox* skyBox;
unsigned int cardTextures[13];
unsigned int grassId;

void load_sky_box()
{
	std::vector<std::string> faces
	{
		/*"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/right.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/left.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/top.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/bottom.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/front.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/back.jpg",*/
		"C:/Dev/assets/coop_platformer_full_asset_location/skybox/right.png",
		"C:/Dev/assets/coop_platformer_full_asset_location/skybox/left.png",
		"C:/Dev/assets/coop_platformer_full_asset_location/skybox/top.png",
		"C:/Dev/assets/coop_platformer_full_asset_location/skybox/bottom.png",
		"C:/Dev/assets/coop_platformer_full_asset_location/skybox/front.png",
		"C:/Dev/assets/coop_platformer_full_asset_location/skybox/back.png"
	};
	skyBox = new SkyBox(faces);
}

unsigned int load_texture(const char* filePath, bool isMipMapped, bool isFlipped) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	if (isFlipped)
	{
		stbi_set_flip_vertically_on_load(0);
	}
	else {
		stbi_set_flip_vertically_on_load(1);
	}
	
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		//mipmap requires power of two sizing that's why there was an issue, look into mipmapping
		if (isMipMapped)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		std::cout << "Loaded texture: " << filePath << std::endl;
	}
	else {
		std::cout << "Failed to load texture: " << filePath << std::endl;
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

unsigned int AssetManager::get_grass()
{
	return grassId;
}

void AssetManager::load()
{
	assetMap[0] = new Model((char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae");
	assetMap[1] = new Model((char*)"C:/Dev/assets/cube/cube.glb");
	assetMap[2] = new Model((char*)"C:/Dev/assets/coop_platformer_full_asset_location/plane.obj");
	assetMap[3] = new Model((char*)"C:/Dev/assets/die.obj");
	assetMap[4] = new Model((char*)"C:/Dev/assets/party_game/stone_portal/stone_platform.obj");
	assetMap[5] = new Model((char*)"C:/Dev/assets/ball/ball.obj");
	assetMap[6] = new Model((char*)"C:/Dev/assets/goal.obj");
	assetMap[7] = new Model((char*)"C:/Dev/assets/GameCube - Mario Party 7 - Sheep/Sheep/sheep.fbx");
	assetMap[8] = new Model((char*)"C:/Dev/assets/party_game/stone_portal/stone_portal.obj");
	assetMap[9] = new Model((char*)"C:/Dev/assets/party_game/stone_portal/stone_portal_map.obj");
	assetMap[10] = new Model((char*)"C:/Dev/assets/party_game/stone_portal/saucer.obj");
	assetMap[11] = new Model((char*)"C:/Dev/assets/sonar/block.obj");
	assetMap[12] = new Model((char*)"C:/Dev/assets/sonar/goldcoin.obj");
	assetMap[13] = new Model((char*)"C:/Dev/assets/coop_platformer_full_asset_location/cylinder.obj");

	load_sky_box();

	uiAssetMap[0] = load_texture("C:/Dev/assets/sonar/crosshair006.png", true, false);
	//grassId = load_texture("C:/Dev/assets/party_game/Billboard_grass/Billboard_grass/sprites/n_grass_diff_0_40.png", true, false);*/
}

Model* AssetManager::get_model(uint8_t id)
{
	return assetMap[id];
}

uint16_t AssetManager::get_num_loaded_assets()
{
	return assetMap.size();
}

SkyBox* AssetManager::get_sky_box()
{
	return skyBox;
}

uint8_t AssetManager::get_card(uint8_t card)
{
	return cardTextures[card];
}

unsigned int AssetManager::get_ui_element(uint16_t id)
{
	return uiAssetMap[id];
}