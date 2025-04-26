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
	Maps.push_back(new Chamber("C:/Dev/opengl_code/Erl/Erl/Game/res/maps/chamber_1"));

	level = 0;
	Maps[level]->load(m_windowWidth, m_windowHeight);
	Maps[level]->set_controls();
	DebugMenu::load_camera(Maps[level]->camera);
	DebugMenu::load_entities(Maps[level]->entities);
}

void Game::update_controls(GameState before, GameState after)
{
	if (before == GAME_ACTIVE)
	{
		Maps[level]->clear_controls();
	}

	if (after == GAME_ACTIVE)
	{
		Maps[level]->set_controls();
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

	/*if (State == DEBUG_MENU)
	{*/
		DebugMenu::create_menu(deltaTime);
	//}
}