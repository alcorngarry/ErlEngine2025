#include"MoveUpCommand.h"

MoveUpCommand::MoveUpCommand(Player* player) : player(player) {}

void MoveUpCommand::execute(float deltaTime) {
    player->Velocity.x = 100.0f;
    player->update(deltaTime);
}