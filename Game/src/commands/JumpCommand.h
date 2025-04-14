#ifndef JUMP_COMMAND_H
#define JUMP_COMMAND_H
#include"input/Command.h"
#include"game_objects/Player.h"

class JumpCommand : public Command {
public:
    JumpCommand(Player* player);
    void execute(float deltaTime) override;

private:
    Player* m_player;
};
#endif // !JUMP_COMMAND_H