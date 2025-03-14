#ifndef MOVE_CAMERA_COMMAND_H
#define MOVE_CAMERA_COMMAND_H
#include<iostream>
#include<commands/SelectEntityCommand.h>
#include"input/Command.h"
#include"common/Camera.h"
#include"input/InputManager.h"

class MoveCameraCommand : public Command {
public:
    MoveCameraCommand(Camera* camera, float deltaTime, uint8_t direction);
    void execute() override;

private:
    Camera* camera;
    uint8_t direction; //0 left, 1 right, 2 up, 3 down
    float deltaTime;
};
#endif // !MOVE_CAMERA_COMMAND_H