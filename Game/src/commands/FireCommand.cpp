#include"FireCommand.h"

FireCommand::FireCommand(Camera* camera) : m_camera(camera) {}

void FireCommand::execute(float deltaTime)
{
	ErlPhysics::Ray* ray = ErlPhysics::cast_ray_from_screen(m_camera);
	Renderer::add_ray(ray);
}