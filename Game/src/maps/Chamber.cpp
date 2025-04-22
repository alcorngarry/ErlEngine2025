#include"Chamber.h"

Chamber::Chamber(std::string mapName) : Map(mapName)
{
}

void Chamber::update(float deltaTime)
{
	if(state == DEFAULT)
	{
		player->update(deltaTime);
		player->floorHeight = ErlPhysics::check_floor_collision(player->Position, entities);
		player->Position += player->Velocity * deltaTime;
	}
}

void Chamber::load_player()
{
	player = new Player(0, AssetManager::get_model(11), camera, glm::vec3(0.0f));
	Renderer::add_render_object(player);
}

void Chamber::load_camera(float windowWidth, float windowHeight)
{
	camera = new Camera(windowWidth, windowHeight);
	UIManager::load_elements();
}

void Chamber::draw(float deltaTime)
{
	Renderer::render(camera);
	UIManager::draw();
}

void Chamber::set_controls()
{
	InputManager::set_mouse_binding(-2, new MoveCameraCommand(camera, CameraMovement::LOOK_AROUND));
	InputManager::set_key_binding(GLFW_KEY_W, new MovePlayerCommand(player, PlayerMovement::FORWARD));
	InputManager::set_key_binding(GLFW_KEY_A, new MovePlayerCommand(player, PlayerMovement::LEFT));
	InputManager::set_key_binding(GLFW_KEY_S, new MovePlayerCommand(player, PlayerMovement::BACK));
	InputManager::set_key_binding(GLFW_KEY_D, new MovePlayerCommand(player, PlayerMovement::RIGHT));
	InputManager::set_key_binding(GLFW_KEY_SPACE, new JumpCommand(player));
	InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new FireCommand(camera));
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