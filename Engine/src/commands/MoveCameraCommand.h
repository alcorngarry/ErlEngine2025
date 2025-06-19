#ifndef MOVE_CAMERA_COMMAND_H
#define MOVE_CAMERA_COMMAND_H
#include<iostream>
#include"input/Command.h"
#include"common/Camera.h"
#include"input/InputManager.h"
#include"math/physics/ErlPhysics.h"

enum CameraMovement {
    MOUSE_DRAG,
    SCROLL,
    ORBIT,
    LOOK_AROUND, 
    LOOK_X,
    LOOK_Y
};

class MoveCameraCommand : public Command {
public:
    MoveCameraCommand(Camera* camera, CameraMovement movement);
    void execute(float deltaTime) override;
    void execute(float deltaTime, float axis) override;

private:
    Camera* camera;
    CameraMovement movement;
};
#endif // !MOVE_CAMERA_COMMAND_H