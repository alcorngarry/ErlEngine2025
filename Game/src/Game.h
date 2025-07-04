#ifndef GAME_H
#define GAME_H
#include"maps/Chamber.h"
#include"common/DebugMenu.h"
#include"common/AssetManager.h"
#include"commands/SaveCommand.h"

enum GameState {
	GAME_ACTIVE,
	DEBUG_MENU,
	MAP_UPDATE
};

class Game {
	public:
		std::vector<Map*> Maps;
		GameState State;
		short level;

		Game(GLFWwindow* window);
		~Game();

		void init();
		void update_controls(GameState before, GameState after);
		void update(float deltaTime);
		void render(float deltaTime);
	private: 
		GLFWwindow* m_window;
};	

#endif // !GAME_H