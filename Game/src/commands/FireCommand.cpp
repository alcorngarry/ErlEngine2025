#include"FireCommand.h"

FireCommand::FireCommand(Camera* camera) : m_camera(camera), m_player(nullptr) {}

FireCommand::FireCommand(Player* player) : m_camera(nullptr), m_player(player) {}

void FireCommand::execute(float deltaTime)
{
	if (m_camera)
	{
		ErlPhysics::add_ray(ErlPhysics::cast_ray_from_screen(m_camera));
	}
	else if (m_player)
	{
		ErlPhysics::add_ray(ErlPhysics::cast_ray_from_player(m_player));
	}
}