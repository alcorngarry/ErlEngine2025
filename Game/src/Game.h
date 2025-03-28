#ifndef GAME_H
#define GAME_H
#include<glm/glm.hpp>
#include"common/Map.h"
#include"maps/BoardMap.h"
#include"maps/PongMap.h"
#include"maps/CountingSheep.h"
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

		Game();
		~Game();

		void init();
		void update(float deltaTime);
		void render(float deltaTime);
	private:
		
};	

#endif // !GAME_H