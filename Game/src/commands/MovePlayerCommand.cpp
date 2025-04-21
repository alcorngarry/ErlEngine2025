#include"MovePlayerCommand.h"

MovePlayerCommand::MovePlayerCommand(Player* player, PlayerMovement movement) : player(player), movement(movement)
{
	isContinuous = true;
}

void MovePlayerCommand::execute(float deltaTime)
{
	// speed is currently set as a float of 100.0
	float cameraSpeed = static_cast<float>(100.0f * deltaTime);
	switch (movement)
	{
		case FORWARD: {
			player->Velocity = player->camera->get_camera_pos() + cameraSpeed * glm::vec3(player->camera->get_camera_front().x, 0.0f, player->camera->get_camera_front().z);
			break;
		}
		case BACK: {
			player->Velocity = player->camera->get_camera_pos() - cameraSpeed * glm::vec3(player->camera->get_camera_front().x, 0.0f, player->camera->get_camera_front().z);
			break;
		}
		case LEFT: {
			player->Velocity = player->camera->get_camera_pos() - glm::normalize(glm::cross(player->camera->get_camera_front(), player->camera->get_camera_up())) * cameraSpeed;
			break;
		}
		case RIGHT: {
			player->Velocity = player->camera->get_camera_pos() + glm::normalize(glm::cross(player->camera->get_camera_front(), player->camera->get_camera_up())) * cameraSpeed;
			break;
		}
	}
}
