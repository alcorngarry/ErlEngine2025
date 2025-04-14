#ifndef MOVE_DOWN_COMMAND_H
#define MOVE_DOWN_COMMAND_H
#include"input/Command.h"
class Player;

class MoveDownCommand : public Command {
public:
    MoveDownCommand(Player* player);
    void execute(float deltaTime) override;

private:
    Player* player;
};
#endif // !MOVE_DOWN_COMMAND_H