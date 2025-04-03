#include"FireCommand.h"

FireCommand::FireCommand(Camera* camera) : m_camera(camera) {}

void FireCommand::execute() 
{
	ErlPhysics::cast_ray_from_screen(m_camera);
}