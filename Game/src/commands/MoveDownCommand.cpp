#include"MoveDownCommand.h"
#include"game_objects/Player.h" 

MoveDownCommand::MoveDownCommand(Player* player) : player(player) {}

void MoveDownCommand::execute(float deltaTime) {
    player->Velocity.x = -100.0f;
    player->update(deltaTime);
}