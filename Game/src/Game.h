#ifndef GAME_H
#define GAME_H
#include<glm/glm.hpp>
#include"common/Map.h"
#include"maps/BoardMap.h"
#include"maps/Chamber.h"
#include"common/AssetManager.h"
#include"commands/SaveCommand.h"
#include"commands/MoveCameraCommand.h"
#include"commands/SelectEntityCommand.h"
#include"commands/AddRemoveEntityCommand.h"
#include"commands/MoveEntityCommand.h"

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

		Game(float windowWidth, float windowHeight);
		~Game();

		void init();
		void update_controls(GameState before, GameState after);
		void update(float deltaTime);
		void render(float deltaTime);
	private: 
		float m_windowWidth, m_windowHeight;
};	

#endif // !GAME_H