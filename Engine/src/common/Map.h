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
		//PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };

		enum State {
			DEFAULT,
			DEBUG
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
		void save();
		virtual void load();
		virtual void draw(float deltaTime);
		virtual void set_controls(float deltaTime) = 0;
		virtual void update(float deltaTime) = 0;
		//move to editor class
		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
		//probably rename and fix.
		virtual void load_skinned_objects() = 0;
	private:
		void load_skybox();
		GameObject* read_asset();
};

#endif // !MAP_H
