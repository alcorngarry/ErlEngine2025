#ifndef FIRE_COMMAND_H
#define FIRE_COMMAND_H
#include<math/physics/ErlPhysics.h>
#include"input/Command.h"

class FireCommand : public Command {
    public:
        FireCommand(Camera* camera);
        void execute(float deltaTime) override;
    private:
        Camera* m_camera;
};
#endif // !FIRE_COMMAND_H