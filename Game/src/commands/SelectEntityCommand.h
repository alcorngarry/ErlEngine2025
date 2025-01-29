#ifndef SELECT_ENTITY_COMMAND_H
#define SELECT_ENTITY_COMMAND_H
#include"input/Command.h"
#include"renderer/Renderer.h"

class SelectEntityCommand : public Command {
public:
    SelectEntityCommand(float xpos, float ypos, bool isDeselect);
    void execute() override;

private:
    float xpos, ypos;
    bool isDeselect;
};
#endif // !SELECT_ENTITY_COMMAND_H