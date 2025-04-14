#include"JumpCommand.h"

JumpCommand::JumpCommand(Player* player) : m_player(player) {}

void JumpCommand::execute(float deltaTime)
{
	m_player->jumping = true;
	m_player->jumpVelocity = m_player->jumpHeight;
}