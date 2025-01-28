#ifndef MOVE_CAMERA_RIGHT_COMMAND_H
#define MOVE_CAMERA_RIGHT_COMMAND_H
#include"input/Command.h"
#include"common/Camera.h"

class MoveCameraRightCommand : public Command {
public:
    MoveCameraRightCommand(Camera* camera, float deltaTime);
    void execute() override;

private:
    Camera* camera;
    float deltaTime;
};
#endif // !MOVE_CAMERA_RIGHT_COMMAND_H