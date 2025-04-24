#include"JumpCommand.h"

JumpCommand::JumpCommand(Player* player) : m_player(player) {}

void JumpCommand::execute(float deltaTime)
{
	m_player->onGround = false;
	m_player->Velocity.y = m_player->jumpForce;
}