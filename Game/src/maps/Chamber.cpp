#include"Chamber.h"

Chamber::Chamber(std::string mapName) : Map(mapName)
{
}

void Chamber::update(float deltaTime)
{
	if(state == ACTIVE)
	{
		set_controls(deltaTime);
	}
	if(!Renderer::get_rays().empty())
	{
		cast_ray_logic(Renderer::get_rays().back());
	}
}

void Chamber::load_skinned_objects()
{
}

void Chamber::load_camera(float windowWidth, float windowHeight)
{
	camera = new Camera(windowWidth, windowHeight);
	camera->set_camera_pos(glm::vec3(0.0f, 2.0f, 0.0f));

	UIManager::load_elements();
}

void Chamber::draw(float deltaTime)
{
	Renderer::render(camera);
	UIManager::draw();
}

void Chamber::cast_ray_logic(Renderer::Ray* ray)
{
	GameObject* selectedObject = ErlPhysics::check_collision(ray, entities);
	if(selectedObject != nullptr) selectedObject->isRendered = false;
}

void Chamber::set_controls(float deltaTime)
{
	InputManager::set_mouse_binding(-2, new MoveCameraCommand(camera, 0.0f, LOOK_AROUND));
	InputManager::set_key_binding(GLFW_KEY_W, new MoveCameraCommand(camera, deltaTime, FORWARD));
	InputManager::set_key_binding(GLFW_KEY_A, new MoveCameraCommand(camera, deltaTime, LEFT));
	InputManager::set_key_binding(GLFW_KEY_S, new MoveCameraCommand(camera, deltaTime, BACK));
	InputManager::set_key_binding(GLFW_KEY_D, new MoveCameraCommand(camera, deltaTime, RIGHT));
	InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new FireCommand(camera));
}