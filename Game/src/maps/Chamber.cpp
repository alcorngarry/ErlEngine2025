#include"Chamber.h"

float radius = 0.0f;

Chamber::Chamber(std::string mapName) : Map(mapName)
{
}

void Chamber::update(float deltaTime)
{
	if (state == DEFAULT)
	{
		for (Player* player : players)
		{
			player->floorHeight = ErlPhysics::check_floor_collision(player);
			player->update(deltaTime);
			if (!InputManager::GamePadConnected[player->playerId] && glfwJoystickIsGamepad(player->playerId)) set_controls(player);
		}
	}

	for (const auto& pair : entities)
	{
		pair.second->update(deltaTime);
	}

	for (const auto& pair : npcs)
	{
		pair.second->floorHeight = ErlPhysics::check_floor_collision(pair.second);
		pair.second->update(deltaTime);
	}
}

void Chamber::draw(float deltaTime)
{
	int i = 0;
	for (Player* player: players)
	{
		if (i == 0)
		{ 
			glViewport(0, 0, cameras[0]->get_window_width() / 2, cameras[0]->get_window_height());
		}
		else {
			glViewport(cameras[1]->get_window_width() / 2, 0, cameras[1]->get_window_width() / 2, cameras[1]->get_window_height());
		}

		if (state != DEBUG)
		{
			
			if (radius != players[0]->camera->followRadius)
			{
				radius = players[0]->camera->followRadius;
				std::cout << "RADIUS: " << player->camera->followRadius << std::endl;
			}
			//resets the camera after editing
			player->camera->followRadius = 200.0f;
			player->camera->follow_position(player->Position);
			UIManager::get_text_element(textId)->text = "W: " + ErlMath::vec3_to_string(player->wishVelocity) + " V: " + ErlMath::vec3_to_string(player->Velocity);
			UIManager::get_text_element(textId2)->text = "Coins: " + std::to_string(player->coinCount);
			
			if (player->health <= 0.0f)
			{
				UIManager::get_text_element(textId3)->color = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			UIManager::get_text_element(textId3)->text = "Health: " + std::to_string(player->health);
			UIManager::get_text_element(textId4)->text = "FPS: " + std::to_string(1.0f / deltaTime);
		}

		Renderer::render(player->camera);
		UIManager::draw();
		i++;
	}
}

void Chamber::set_controls(Player* player)
{
	Map::set_controls(player);
	InputManager::set_gamepad_binding({ player->playerId, 100 - GLFW_GAMEPAD_AXIS_RIGHT_X }, new MoveCameraCommand(player->camera, CameraMovement::LOOK_X));
	InputManager::set_gamepad_binding({ player->playerId, 100 - GLFW_GAMEPAD_AXIS_RIGHT_Y }, new MoveCameraCommand(player->camera, CameraMovement::LOOK_Y));
	InputManager::set_gamepad_binding({ player->playerId, 100 - GLFW_GAMEPAD_AXIS_LEFT_X }, new MovePlayerCommand(player, PlayerMovement::GAMEPAD_X));
	InputManager::set_gamepad_binding({ player->playerId, 100 - GLFW_GAMEPAD_AXIS_LEFT_Y }, new MovePlayerCommand(player, PlayerMovement::GAMEPAD_Y));
	InputManager::set_gamepad_binding({ player->playerId, GLFW_GAMEPAD_BUTTON_A }, new JumpCommand(player));
	InputManager::set_gamepad_binding({ player->playerId, GLFW_GAMEPAD_BUTTON_X }, new FireCommand(player));
}