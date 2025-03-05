#include"Game.h"

Game::Game() : State(GAME_ACTIVE)
{
}

Game::~Game()
{
}

void Game::init()
{
	AssetManager::load();
	Maps.push_back(new BoardMap("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/test_map_1"));
	Maps.push_back(new PongMap("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/test_map_2"));
	Maps.push_back(new CountingSheep("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/counting_sheep"));
	Maps.push_back(new PongMap("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/mini_game_1"));
	level = 0;
	Maps[level]->load();
	Maps[level]->set_controls(0.0f);
}

void Game::update(float deltaTime)
{ 
	InputManager::set_key_binding(GLFW_KEY_W, new MoveCameraCommand(Maps[level]->camera, deltaTime, 0));
	InputManager::set_key_binding(GLFW_KEY_A, new MoveCameraCommand(Maps[level]->camera, deltaTime, 2));
	InputManager::set_key_binding(GLFW_KEY_S, new MoveCameraCommand(Maps[level]->camera, deltaTime, 1));
	InputManager::set_key_binding(GLFW_KEY_D, new MoveCameraCommand(Maps[level]->camera, deltaTime, 3));

	if (State == DEBUG_MENU)
	{
		Maps[level]->state = Map::DEBUG;
		
		InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new SelectEntityCommand(InputManager::get_xpos(), InputManager::get_ypos(), false));
		InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_RIGHT, new SelectEntityCommand(0.0f, 0.0f, true));
		InputManager::set_key_binding(GLFW_KEY_N, new AddRemoveEntityCommand(Maps[level], true));
		InputManager::set_key_binding(GLFW_KEY_R, new AddRemoveEntityCommand(Maps[level], false));

		//InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new MoveCameraCommand(Maps[level]->camera, deltaTime, 4));
		//Maps[level]->camera->set_camera_front(glm::normalize(glm::vec3(cos(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch())), sin(glm::radians(InputManager::get_pitch())), sin(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch())))));

		if (Renderer::get_selected_index() != -1)
		{
			InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_LEFT);
			/*InputManager::set_key_binding(GLFW_KEY_LEFT, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 0, 3.0f));
			InputManager::set_key_binding(GLFW_KEY_RIGHT, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 0, -3.0f));
			InputManager::set_key_binding(GLFW_KEY_UP, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 1, 3.0f));
			InputManager::set_key_binding(GLFW_KEY_DOWN, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 1, -3.0f));
			InputManager::set_key_binding(GLFW_KEY_J, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 2, 3.0f));
			InputManager::set_key_binding(GLFW_KEY_K, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 2, -3.0f));*/
		}
		else {
			Maps[level]->camera->set_camera_front(glm::normalize(glm::vec3(cos(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch())), sin(glm::radians(InputManager::get_pitch())), sin(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch())))));
		}
	}
	else {
		Maps[level]->state = Map::DEFAULT;
	}
	
	if (Maps[level]->loadState == Map::LoadState::CHANGE_MAP)
	{
		level++;
		Maps[level]->load();
	}
	
	Maps[level]->update(deltaTime);
}

void Game::render(float deltaTime)
{
	Maps[level]->draw(deltaTime);

	if (State == DEBUG_MENU)
	{
		Renderer::create_menu(deltaTime);
	}
} 