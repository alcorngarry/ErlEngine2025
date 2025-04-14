#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H
#include"input/Command.h"
#include"game_objects/Player.h"

class MoveUpCommand : public Command {
public:
    MoveUpCommand(Player* player);
    void execute(float deltaTime) override;

private:
    Player* player;
    float deltaTime;
};
#endif // !MOVE_UP_COMMAND_H