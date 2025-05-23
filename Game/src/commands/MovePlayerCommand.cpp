#include"MovePlayerCommand.h"

MovePlayerCommand::MovePlayerCommand(Player* player, PlayerMovement movement) : player(player), movement(movement)
{
	isContinuous = true;
}

void MovePlayerCommand::execute(float deltaTime)
{
	switch (movement)
	{
		case FORWARD: {
			player->fmove += 200.0f * 0.25f;
			break;
		}
		case BACK: {
			player->fmove -= 200.0f * 0.25f;
			break;
		}
		case LEFT: {
			player->smove -= 320.0f * 0.25f;
			break;
		}
		case RIGHT: {
			player->smove += 320.0f * 0.25f;
			break;
		}
	}
}

void MovePlayerCommand::execute(float deltaTime, float axis)
{
	switch (movement)
	{
		case GAMEPAD_X: {
			player->smove += 100.0f * axis;
			break;
		}
		case GAMEPAD_Y: {
			player->fmove -= 160.0f * axis;
			break;
		}
	}
}
