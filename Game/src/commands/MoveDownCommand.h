#ifndef MOVE_DOWN_COMMAND_H
#define MOVE_DOWN_COMMAND_H
#include"input/Command.h"
class Player;

class MoveDownCommand : public Command {
public:
    MoveDownCommand(Player* player, float deltaTime);
    void execute() override;

private:
    Player* player;
    float deltaTime;
};
#endif // !MOVE_DOWN_COMMAND_H