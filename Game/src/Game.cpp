#include"Game.h"

Game::Game(GLFWwindow* window) : State(GAME_ACTIVE), m_window(window)
{
}

Game::~Game()
{
}

void Game::init()
{
	AssetManager::load();
	Maps.push_back(new Chamber("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/chamber_1"));

	level = 0;
	Maps[level]->load(m_window);
	DebugMenu::load_map(Maps[level]);
	UIManager::load_map(Maps[level]);
}

void Game::update_controls(GameState before, GameState after)
{
	if (before == GAME_ACTIVE)
	{
		Maps[level]->clear_controls();
		DebugMenu::set_controls();
	}

	if (after == MAP_UPDATE)
	{
		DebugMenu::clear_controls();
		for (Player* player : Maps[level]->players)
		{
			Maps[level]->set_controls(player);
		}
		State = GAME_ACTIVE;
	}
}

void Game::update(float deltaTime)
{ 
	if (State == DEBUG_MENU)
	{
		Maps[level]->state = Map::DEBUG;
		InputManager::show_cursor(true);
	}
	else if (State == MAP_UPDATE)
	{
		Maps[level]->state = Map::MAP_UPDATE;
		State = GAME_ACTIVE;
	}
	else {
		Maps[level]->state = Map::DEFAULT;
		InputManager::show_cursor(false);
	}
	
	Maps[level]->update(deltaTime);
}

void Game::render(float deltaTime)
{
	Maps[level]->draw(deltaTime);

	if (State == DEBUG_MENU)
	{
		DebugMenu::create_menu(deltaTime);
	}
}