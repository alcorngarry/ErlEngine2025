#include"BoardMap.h"


BoardMap::BoardMap(std::string mapName) : Map(mapName)
{
}

void BoardMap::update(float deltaTime)
{
	set_controls(deltaTime);
	update_camera_position(deltaTime);
	display_cards();
	if (state == MAP_UPDATE)
	{
		update_board_space_directions();
		state = DEFAULT;
	}
}

void BoardMap::draw(float deltaTime)
{
	Renderer::render(camera);
	UIManager::draw();
}

void BoardMap::load()
{
	Map::load();
	load_skinned_objects();
}

//remove delta time after fixing animation call structure
void BoardMap::set_controls(float deltaTime)
{
	//rework gamepad binding to bind player to gamepad.
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

void BoardMap::write_board_spaces()
{
	writeMap << std::endl;
	writeMap << "boardSpaces: [";
	for (int i = 0; i < boardSpaces.size(); i++)
	{
		writeMap << "{";
		writeMap << "uniqueId: " << boardSpaces[i]->id << ", " 
			<< "spaceType: " << static_cast<int>(boardSpaces[i]->spaceType) << ", "
			<< "nextSpaces: " << "[" << boardSpaces[i]->nextSpace[0]->id << "], "
			<< "position: " << boardSpaces[i]->Position.x << "," << boardSpaces[i]->Position.y << "," << boardSpaces[i]->Position.z << ", "
			<< "scale: " << boardSpaces[i]->Size.x << "," << boardSpaces[i]->Size.y << "," << boardSpaces[i]->Size.z << ", "
			<< "rotation: " << boardSpaces[i]->Rotation.x << "," << boardSpaces[i]->Rotation.y << "," << boardSpaces[i]->Rotation.z;
		if (i == boardSpaces.size() - 1)
		{
			writeMap << "}";
		}
		else {
			writeMap << "}," << std::endl;
		}
	}
	writeMap << "]";
}

void BoardMap::read_board_spaces()
{
	while (readMap.peek() != ']')
	{
		BoardSpace* entity = read_board_space();
		boardSpaces.push_back(entity);
		Renderer::add_render_object(entity);
	}
	update_board_space_directions();
}

void BoardMap::update_board_space_directions()
{
	for (BoardSpace* boardSpace : boardSpaces)
	{
		if (!boardSpace->nextSpace.empty())
		{
			boardSpace->nextSpace[0] = boardSpaces[boardSpace->nextSpaceIds[0]];
		}
		else {
			boardSpace->nextSpace.push_back(boardSpaces[boardSpace->nextSpaceIds[0]]);
		}
	}
}

BoardSpace* BoardMap::read_board_space()
{
	std::string line;
	getline(readMap, line, ':');
	uint8_t uniqueId, spaceType;
	float x, y, z;
	glm::vec3 position, scale, rotation;
	std::vector<int> nextSpaceIds;

	//uniqueId
	getline(readMap, line, ',');
	uniqueId = std::stof(line);

	//spaceType
	getline(readMap, line, ':');
	getline(readMap, line, ',');
	spaceType = std::stof(line);

	//nextSpaceIds
	getline(readMap, line, ':');
	getline(readMap, line, '[');

	if (readMap.peek() != ']')
	{
		getline(readMap, line, ',');
		nextSpaceIds.push_back(std::stof(line));
	} else {
		getline(readMap, line, ']');
		nextSpaceIds.push_back(std::stof(line));
		getline(readMap, line, ',');
	}

	getline(readMap, line, ':');

	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, ',');
	z = std::stof(line);
	position = { x, y, z };

	getline(readMap, line, ':');

	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, ',');
	z = std::stof(line);
	scale = { x, y, z };

	getline(readMap, line, ':');

	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, '}');
	z = std::stof(line);
	rotation = { x, y, z };

	BoardSpace* space = new BoardSpace(boardSpaces.size(), AssetManager::get_model(4), position, scale, rotation);
	space->nextSpaceIds = nextSpaceIds;

	return space;
}

void BoardMap::save()
{
	entities = Renderer::get_rendered_entities();
	writeMap = std::ofstream{ fileName + ".esf" };

	entities.resize(entities.size() - boardSpaces.size());

	write_models();
	write_lights();
	write_board_spaces();

	writeMap.close();
}