#ifndef MAP_H
#define MAP_H
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<glm/glm.hpp>
#include"renderer/Renderer.h"
#include"AssetManager.h"
#include"input/InputManager.h"

class Map 
{
	public:
		enum State {
			DEFAULT,
			DEBUG,
			MAP_UPDATE
		};

		enum LoadState {
			KEEP_MAP,
			CHANGE_MAP
		};

		LoadState loadState;
		State state;
		std::vector<GameObject*> entities;
		std::vector<GameObject*> lights;
		std::vector<SkinnedGameObject*> skinned_entities;
		Camera* camera;

		Map(std::string mapName);
		virtual void save();
		virtual void load(float windowWidth, float windowHeight);
		virtual void draw(float deltaTime);
		virtual void set_controls(float deltaTime) = 0;
		virtual void update(float deltaTime) = 0;
		void read_models();
		void read_lights();
		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
		virtual void load_skinned_objects() = 0;
		virtual void load_camera(float windowWidth, float windowHeight);
		virtual void read_board_spaces() {};
		void write_models();
		void write_lights();
	private:
		std::string line;
		void load_skybox();
		GameObject* read_asset();
};

#endif // !MAP_H
