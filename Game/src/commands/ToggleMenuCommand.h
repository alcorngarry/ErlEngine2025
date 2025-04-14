#ifndef TOGGLE_MENU_COMMAND_H 
#define TOGGLE_MENU_COMMAND_H
#include"input/Command.h"
#include"Game.h"

class ToggleMenuCommand : public Command {
public:
    ToggleMenuCommand(Game* game);
    void execute(float deltaTime) override;

private:
    Game* game;
};
#endif // !TOGGLE_MENU_COMMAND_H