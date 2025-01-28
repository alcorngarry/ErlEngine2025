#include"MoveDownCommand.h"
#include"game_objects/Player.h" 

MoveDownCommand::MoveDownCommand(Player* player, float deltaTime) : player(player), deltaTime(deltaTime) {}

void MoveDownCommand::execute() {
    player->velocity.x = -100.0f;
    player->update(deltaTime);
}