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
			
		}
	}

	for (const auto& pair : entities)
	{
		pair.second->update(deltaTime);
	}
}

void Chamber::load_camera(float windowWidth, float windowHeight)
{
	camera = new Camera(windowWidth, windowHeight);
	ErlPhysics::add_physics_camera(camera);
	UIManager::load_elements();
}

void Chamber::draw(float deltaTime)
{
	//if (renderState == PLAYER1)
	//{
	if (state != DEBUG)
	{
		if (radius != camera->followRadius)
		{
			radius = camera->followRadius;
			std::cout << "RADIUS: " << camera->followRadius << std::endl;
		}
		//resets the camera after editing
		camera->followRadius = 200.0f;
		camera->follow_position(players[0]->Position);
		UIManager::get_text_element(textId)->text = "W: " + ErlMath::vec3_to_string(players[0]->wishVelocity) + " V: " + ErlMath::vec3_to_string(players[0]->Velocity);
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
	Map::set_controls();
	if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
	{
		//set arbitrary value for joystick
	
		int i = 0;
		for (Player* player : players)
		{
			InputManager::set_gamepad_binding({ i, 100 - GLFW_GAMEPAD_AXIS_RIGHT_X }, new MoveCameraCommand(camera, CameraMovement::LOOK_X));
			InputManager::set_gamepad_binding({ i, 100 - GLFW_GAMEPAD_AXIS_RIGHT_Y }, new MoveCameraCommand(camera, CameraMovement::LOOK_Y));
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