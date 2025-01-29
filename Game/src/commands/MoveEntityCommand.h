#ifndef MOVE_ENTITY_COMMAND_H
#define MOVE_ENTITY_COMMAND_H
#include"input/Command.h"
#include"game_objects/GameObject.h"

class MoveEntityCommand : public Command {
public:
    // for right now  axis = 0:x, 1:y, 2:z
    MoveEntityCommand(GameObject* entity, int axis, float moveSpeed);
    void execute() override;

private:
    GameObject* entity;
    int axis;
    float moveSpeed;
};
#endif // !MOVE_ENTITY_COMMAND_H