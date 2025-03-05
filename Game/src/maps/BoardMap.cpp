#include"BoardMap.h"

BoardMap::BoardMap(std::string mapName) : Map(mapName)
{
}

void BoardMap::update(float deltaTime)
{
	set_controls(deltaTime);
	update_camera_position(deltaTime);
	display_cards();
}

void BoardMap::draw(float deltaTime)
{
	Renderer::render(camera);
	UIManager::draw();
}

void BoardMap::load()
{
	Map::load();
	init_board_spaces();
	load_skinned_objects();
}

//remove delta time after fixing animation call structure
void BoardMap::set_controls(float deltaTime)
{
	//rework gamepad binding to bind player to gamepad.
	/*InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_UP, new MoveUpCommand(players[currentPlayer], deltaTime));
	InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_DOWN, new MoveDownCommand(players[currentPlayer], deltaTime));*/
	InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, new SelectCardLeftCommand(players[currentPlayer]));
	InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, new SelectCardRightCommand(players[currentPlayer]));
	InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_A, new SelectCardCommand(players[currentPlayer]));
}

void BoardMap::load_skinned_objects()
{
	glm::vec3 startingPosition = boardSpaces[0]->Position + glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 1; i++)
	{
		Player* player = new Player(i, AssetManager::get_model(0), startingPosition, glm::vec3(2.0f), glm::vec3(0.0f), playerControls);
		players.push_back(player);
		Renderer::add_skinned_render_object(player);
		players[i]->init_deck();
		players[i]->currSpace = boardSpaces[0];

		if (i == 0)
		{
			players[i]->state = ACTIVE;
			players[currentPlayer] = players[i];
			UIManager::add_text_element("Player: " + std::to_string(currentPlayer) + ", Groats: " + std::to_string(players[currentPlayer]->groats), 0.0f, 0.0f);
		}
	}
}

void BoardMap::process_board_space(uint8_t boardId)
{
	switch (boardId)
	{
		case 0: 
			//loadState = CHANGE_MAP;
			break;
		case 1:
			players[currentPlayer]->add_groats(3);
			std::cout << "added 3 groats to player: " << players[currentPlayer]->id << ", total groats = " << players[currentPlayer]->groats << std::endl;
			break;
		case 2:
			players[currentPlayer]->add_groats(6);
			std::cout << "added 6 groats to player: " << players[currentPlayer]->id  << ", total groats = " << players[currentPlayer]->groats << std::endl;
			break;
		case 3:
			players[currentPlayer]->remove_groats(3);
			std::cout << "removed 3 groats to player: " << players[currentPlayer]->id << ", total groats = " << players[currentPlayer]->groats << std::endl;
			break;
		default: 
			std::cout << "error processing space" << std::endl;
	}
	UIManager::get_text_element(currentPlayer)->text = "Player: " + std::to_string(currentPlayer) + ", Groats: " + std::to_string(players[currentPlayer]->groats);
}

void BoardMap::display_cards()
{
	if (players[currentPlayer]->state == IN_MOTION) {
		uint8_t card[1] = { players[currentPlayer]->get_cards()[players[currentPlayer]->get_selected_card_index()] };
		
		UIManager::load_elements(card, 6);
	}
	else {
		UIManager::load_elements(players[currentPlayer]->get_cards(), players[currentPlayer]->get_selected_card_index());
	}
}

void BoardMap::init_board_spaces()
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->id == 4)
		{
			boardSpaces.push_back(new BoardSpace(entities[i]));
		}
	}
	
	for (size_t i = 0; i < boardSpaces.size(); i++)
	{
		//adding buffer for character height, may need to be changed
		boardSpaces[i]->Position + glm::vec3(0.0f, 1.0f, 0.0f);
		if (i + 1 == boardSpaces.size())
		{
			boardSpaces[i]->nextSpace.push_back(boardSpaces[0]);
		}
		else 
		{
			boardSpaces[i]->nextSpace.push_back(boardSpaces[i + 1]);
		}
	}
}

//this also moves the player, fix
void BoardMap::update_camera_position(float deltaTime)
{
	// Update the camera's position
	// tan = a/h, h is what we want.
	// a over tan gives u h
	if (state == DEFAULT)
	{
		if (players[currentPlayer]->state == IN_MOTION)
		{
			float playerYRotation = glm::radians(players[currentPlayer]->Rotation.y);
			glm::vec3 playerDirection = glm::vec3(sin(playerYRotation), 0.0f, cos(playerYRotation));
			float distanceBehind = 80.0f;
			float heightAbove = 30.0f;
			glm::vec3 cameraOffset = (playerDirection * -distanceBehind) + glm::vec3(0.0f, heightAbove, 0.0f);

			camera->set_camera_pos(players[currentPlayer]->Position + cameraOffset);
			players[currentPlayer]->update(deltaTime);
			players[currentPlayer]->move_player();
		}
		else {
			glm::vec3 direction = players[currentPlayer]->Position - camera->get_camera_pos();
			players[currentPlayer]->Rotation = -1.0f * glm::vec3(0.0f, glm::degrees(std::atan2(direction.x, direction.z)), 0.0f);
		}

		camera->set_camera_front(players[currentPlayer]->Position - camera->get_camera_pos());

		if (players[currentPlayer]->state == TURN_COMPLETE)
		{
			//process_board_space(players[currentPlayer]->currSpace->spaceType);
			players[currentPlayer]->state = INACTIVE;
			currentPlayer = currentPlayer == players.size() - 1 ? 0 : currentPlayer += 1;
			players[currentPlayer]->state = ACTIVE;
		}
	}
}