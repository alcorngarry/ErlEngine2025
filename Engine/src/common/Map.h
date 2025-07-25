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
#include"game_objects/Player.h"
#include"commands/ToggleConsoleCommand.h"
#include"game_objects/NPC.h"

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

		enum RenderState {
			PLAYER1,
			PLAYER2
		};

		LoadState loadState;
		State state;
		RenderState renderState;
		std::map<uint16_t, GameObject*> entities, lights;
		std::map<uint16_t, NPC*> npcs;
		std::vector<SkinnedGameObject*> skinned_entities;
		std::vector<Player*> players;
		std::vector<Camera*> cameras;
		std::map<uint16_t, glm::vec3> playerStarts;

		Map(std::string mapName);
		virtual void save();
		virtual void load(GLFWwindow* window);
		virtual void draw(float deltaTime);
		virtual void set_controls(Player* player);
		virtual void clear_controls();
		virtual void update(float deltaTime) = 0;
		
		void read_models();
		void read_lights();
		void duplicate_model(int selectedIndex);
		GameObject* get_entity_by_instance_id(uint16_t id);
		void remove_model(int selectedIndex);
		void toggle_render(int index);
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
		int textId, textId2, textId3, textId4;

		void read_player();
		void load_physics_objects();
		void write_models();
		void write_lights();
		void write_scripts(GameObject* entity);
		void write_player();
	private:
		GLFWwindow* m_window;
		int m_windowWidth, m_windowHeight;
		std::string line;
		void load_skybox();
		GameObject* read_asset();
		void read_script(GameObject* entity);
		Player* read_player_asset();
};

struct Scripts
{
	static void init();
	static void rotate(GameObject* gameObject, float deltaTime);
	static void hide(GameObject* gameObject, float deltaTime);
	static void show(GameObject* gameObject, float deltaTime);
	static void addPoint(GameObject* player, float deltaTime);
	static std::function<void(GameObject*, float)> get_script(std::string scriptName);
};

#endif // !MAP_H
