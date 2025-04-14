#ifndef MOVE_PLAYER_COMMAND_H
#define MOVE_PLAYER_COMMAND_H
#include"input/Command.h"
#include"game_objects/Player.h"

enum PlayerMovement {
    FORWARD,
    BACK,
    LEFT,
    RIGHT
};

class MovePlayerCommand : public Command {
public:
    MovePlayerCommand(Player* player, PlayerMovement movement);
    void execute(float deltaTime) override;
private:
    Player* player;
    PlayerMovement movement;
    float deltaTime;
};
#endif // !MOVE_PLAYER_COMMAND_H