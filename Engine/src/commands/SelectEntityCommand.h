#ifndef SELECT_ENTITY_COMMAND_H
#define SELECT_ENTITY_COMMAND_H
#include"input/Command.h"
#include"renderer/Renderer.h"
#include"input/InputManager.h"

class SelectEntityCommand : public Command {
public:
    SelectEntityCommand(bool isDeselect);
    void execute(float deltaTime) override;
private:
    bool isDeselect;
};
#endif // !SELECT_ENTITY_COMMAND_H