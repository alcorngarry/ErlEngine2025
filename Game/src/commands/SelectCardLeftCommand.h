#ifndef SELECT_CARD_L_COMMAND_H
#define SELECT_CARD_L_COMMAND_H
#include"input/Command.h"
#include"game_objects/Player.h"

class SelectCardLeftCommand : public Command {
public:
    SelectCardLeftCommand(Player* player);
    void execute() override;

private:
    Player* player;
};
#endif // !SELECT_CARD_L_COMMAND_H