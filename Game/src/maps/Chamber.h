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
		Chamber(std::string mapName);
		void set_controls(Player* player) override;
		void update(float deltaTime) override;
	protected:
		void draw(float deltaTime) override;
};
#endif // !CHAMBER_H
