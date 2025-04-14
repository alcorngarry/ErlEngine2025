#include"MoveUpCommand.h"

MoveUpCommand::MoveUpCommand(Player* player) : player(player) {}

void MoveUpCommand::execute(float deltaTime) {
    player->velocity.x = 100.0f;
    player->update(deltaTime);
}