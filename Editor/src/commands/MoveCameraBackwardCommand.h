#ifndef MOVE_CAMERA_BACKWARD_COMMAND_H
#define MOVE_CAMERA_BACKWARD_COMMAND_H
#include"input/Command.h"
#include"common/Camera.h"

class MoveCameraBackwardCommand : public Command {
public:
    MoveCameraBackwardCommand(Camera* camera, float deltaTime);
    void execute() override;

private:
    Camera* camera;
    float deltaTime;
};
#endif // !MOVE_CAMERA_BACKWARDS_COMMAND_H