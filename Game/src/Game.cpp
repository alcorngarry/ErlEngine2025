#include"Game.h"

Game::Game(float windowWidth, float windowHeight) : State(GAME_ACTIVE), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
}

Game::~Game()
{
}

void Game::init()
{
	AssetManager::load();
	//Maps.push_back(new Chamber("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/test_map_1"));
	Maps.push_back(new BoardMap("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/test_map_1"));

	level = 0;
	Maps[level]->load(m_windowWidth, m_windowHeight);
	Maps[level]->set_controls(0.0f);
}

void Game::update_controls(GameState before, GameState after)
{
	switch (before)
	{
		case DEBUG_MENU: {
			InputManager::remove_key_and_mouse_binding(GLFW_KEY_LEFT_SHIFT, GLFW_MOUSE_BUTTON_MIDDLE);
			InputManager::remove_mouse_binding(-1);
			InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_LEFT);
			InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_RIGHT);
			InputManager::remove_key_binding(GLFW_KEY_N);
			InputManager::remove_key_binding(GLFW_KEY_R);
			break;
		}
		default: break;
	}
	switch (after)
	{
		case DEBUG_MENU: {
			InputManager::set_key_and_mouse_binding(GLFW_KEY_LEFT_SHIFT, GLFW_MOUSE_BUTTON_MIDDLE, new MoveCameraCommand(Maps[level]->camera, 0.0f, MOUSE_DRAG));
			InputManager::set_mouse_binding(-1, new MoveCameraCommand(Maps[level]->camera, 0.0f, SCROLL));
			InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new SelectEntityCommand(false));
			InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_RIGHT, new SelectEntityCommand(true));
			InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_MIDDLE, new MoveCameraCommand(Maps[level]->camera, 0.0f, ORBIT));
			InputManager::set_key_binding(GLFW_KEY_N, new AddRemoveEntityCommand(Maps[level], true));
			InputManager::set_key_binding(GLFW_KEY_R, new AddRemoveEntityCommand(Maps[level], false));
			break;
		}
		default: break;
	}
}

void Game::update(float deltaTime)
{ 
	if (State == DEBUG_MENU)
	{
		Maps[level]->state = Map::DEBUG;

	}
	else if (State == MAP_UPDATE)
	{
		Maps[level]->state = Map::MAP_UPDATE;
		State = GAME_ACTIVE;
	}
	else {
		Maps[level]->state = Map::DEFAULT;
	}
	
	if (Maps[level]->loadState == Map::LoadState::CHANGE_MAP)
	{
		level++;
		Maps[level]->load(m_windowWidth, m_windowHeight);
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