#ifndef GAME_H
#define GAME_H
#include<glm/glm.hpp>
#include"common/Map.h"
#include"maps/BoardMap.h"
#include"maps/PongMap.h"
#include"common/AssetManager.h"
#include"commands/SaveCommand.h"
#include"commands/MoveCameraBackwardCommand.h"
#include"commands/MoveCameraForwardCommand.h"
#include"commands/MoveCameraLeftCommand.h"
#include"commands/MoveCameraRightCommand.h"
#include"commands/SelectEntityCommand.h"
#include"commands/AddRemoveEntityCommand.h"
#include"commands/MoveEntityCommand.h"

enum GameState {
	GAME_ACTIVE,
	DEBUG_MENU
};

class Game {
	public:
		std::vector<Map*> Maps;
		GameState State;
		short level;

		Game(GLFWwindow* window);
		~Game();

		void init();
		void update(float deltaTime);
		void render(float deltaTime);
		void set_debug_controls(float deltaTime);
	private:
		
};	

#endif // !GAME_H