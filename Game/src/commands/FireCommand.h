#ifndef FIRE_COMMAND_H
#define FIRE_COMMAND_H
#include"input/Command.h"
#include"renderer/Renderer.h"
#include"input/InputManager.h"

class FireCommand : public Command {
public:
    FireCommand();
    void execute() override;
private:
};
#endif // !FIRE_COMMAND_H