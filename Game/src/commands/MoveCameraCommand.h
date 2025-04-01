#ifndef MOVE_CAMERA_COMMAND_H
#define MOVE_CAMERA_COMMAND_H
#include<iostream>
#include<commands/SelectEntityCommand.h>
#include"input/Command.h"
#include"common/Camera.h"
#include"input/InputManager.h"

enum Movement {
    FORWARD,
    BACK,
    LEFT,
    RIGHT,
    MOUSE_DRAG,
    SCROLL,
    ORBIT,
    LOOK_AROUND
};

class MoveCameraCommand : public Command {
public:
    MoveCameraCommand(Camera* camera, float deltaTime, Movement movement);
    void execute() override;

private:
    Camera* camera;
    Movement movement;
    float deltaTime;
};
#endif // !MOVE_CAMERA_COMMAND_H