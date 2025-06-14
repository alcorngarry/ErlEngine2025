#include"Chamber.h"

Chamber::Chamber(std::string mapName) : Map(mapName)
{
}

void rotate(GameObject* gameObject, float deltaTime)
{
	gameObject->ModelMatrix = glm::rotate(gameObject->ModelMatrix, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Chamber::update(float deltaTime)
{
	if (state == DEFAULT)
	{
		for (Player* player : players)
		{
			player->floorHeight = ErlPhysics::check_floor_collision(player);
			player->update(deltaTime);
		}
	}

	for (const auto& pair : entities)
	{
		if (pair.second->assetId == 12)
		{
			GameObject* a = pair.second;
			pair.second->actions["rotate"] = [a, deltaTime]() { rotate(a, deltaTime); };
		}
	}

	for (const auto& pair : entities)
	{
		pair.second->update(deltaTime);
	}
}

void Chamber::load_player()
{
	players.push_back(new Player(0, AssetManager::get_model(11), camera, glm::vec3(0.0f)));
	Renderer::add_render_object(players[0]);
	ErlPhysics::add_player_physics_object(players[0]);

	players.push_back(new Player(1, AssetManager::get_model(11), camera, glm::vec3(0.0f)));
	Renderer::add_render_object(players[1]);
	ErlPhysics::add_player_physics_object(players[1]);
}

void Chamber::load_camera(float windowWidth, float windowHeight)
{
	camera = new Camera(windowWidth, windowHeight);
	UIManager::load_elements();
}

void Chamber::draw(float deltaTime)
{
	//if (renderState == PLAYER1)
	//{
	if (state != DEBUG)
	{
		camera->follow_position(players[0]->Position);
	}
		//renderState = PLAYER2;
	/*}
	else if (renderState == PLAYER2)
	{
		camera->follow_position(players[1]->Position);
		renderState = PLAYER1;
	}*/

	Renderer::render(camera);
	UIManager::draw();
}

void Chamber::set_controls()
{
	if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
	{
		//set arbitrary value for joystick
		/*InputManager::set_gamepad_binding({ 0, 100 - GLFW_GAMEPAD_AXIS_RIGHT_X }, new MoveCameraCommand(camera, CameraMovement::LOOK_X));
		InputManager::set_gamepad_binding({ 0, 100 - GLFW_GAMEPAD_AXIS_RIGHT_Y }, new MoveCameraCommand(camera, CameraMovement::LOOK_Y));*/
		int i = 0;
		for (Player* player : players)
		{
			InputManager::set_gamepad_binding({ i, 100 - GLFW_GAMEPAD_AXIS_LEFT_X }, new MovePlayerCommand(player, PlayerMovement::GAMEPAD_X));
			InputManager::set_gamepad_binding({ i, 100 - GLFW_GAMEPAD_AXIS_LEFT_Y }, new MovePlayerCommand(player, PlayerMovement::GAMEPAD_Y));
			InputManager::set_gamepad_binding({ i, GLFW_GAMEPAD_BUTTON_A }, new JumpCommand(player));
			InputManager::set_gamepad_binding({ i, GLFW_GAMEPAD_BUTTON_X }, new FireCommand(player));
			i++;
		}
	}
	else {
	/*	InputManager::set_mouse_binding(-2, new MoveCameraCommand(camera, CameraMovement::LOOK_AROUND));
		InputManager::set_key_binding(GLFW_KEY_W, new MovePlayerCommand(player, PlayerMovement::FORWARD));
		InputManager::set_key_binding(GLFW_KEY_A, new MovePlayerCommand(player, PlayerMovement::LEFT));
		InputManager::set_key_binding(GLFW_KEY_S, new MovePlayerCommand(player, PlayerMovement::BACK));
		InputManager::set_key_binding(GLFW_KEY_D, new MovePlayerCommand(player, PlayerMovement::RIGHT));
		InputManager::set_key_binding(GLFW_KEY_SPACE, new JumpCommand(player));
		InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new FireCommand(camera));*/
	}
}

void Chamber::clear_controls()
{
	InputManager::remove_mouse_binding(-2);
	InputManager::remove_key_binding(GLFW_KEY_W);
	InputManager::remove_key_binding(GLFW_KEY_A);
	InputManager::remove_key_binding(GLFW_KEY_S);
	InputManager::remove_key_binding(GLFW_KEY_D);
	InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_LEFT);
}