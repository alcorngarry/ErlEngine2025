#ifndef TOGGLE_CONSOLE_COMMAND_H
#define TOGGLE_CONSOLE_COMMAND_H
#include"input/Command.h"
#include"ui/UIManager.h"
#include"input/InputManager.h"

class ToggleConsoleCommand : public Command {
public:
    ToggleConsoleCommand();
    void execute(float deltaTime) override;
private:
};
#endif // !TOGGLE_CONSOLE_COMMAND_H