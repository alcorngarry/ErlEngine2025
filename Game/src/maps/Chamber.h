#ifndef CHAMBER_H
#define CHAMBER_H
#include"common/Map.h"
#include"game_objects/Player.h"
#include"commands/MovePlayerCommand.h"
#include"commands/FireCommand.h"
#include"commands/JumpCommand.h"
#include<commands/MoveCameraCommand.h>

class Chamber : public Map {
	public:
		std::vector<Player*> players;
		Chamber(std::string mapName);
		void set_controls() override;
		void update(float deltaTime) override;
	protected:
		void load_player() override;
		void load_camera(float windowWidth, float windowHeight) override;
		void draw(float deltaTime) override;
};
#endif // !CHAMBER_H
