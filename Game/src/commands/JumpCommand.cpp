#include"JumpCommand.h"

JumpCommand::JumpCommand(Player* player) : m_player(player) {}

void JumpCommand::execute(float deltaTime)
{
	if (m_player->onGround)
	{
		m_player->Velocity.y = m_player->jumpForce;
		m_player->onGround = false;
	}
}