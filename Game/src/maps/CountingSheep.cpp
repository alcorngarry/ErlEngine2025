#include"CountingSheep.h"

int counts[1];
int i = 0;

class Restart : public Command {
public:
	Restart(float* startTime) {
		m_startTime = startTime;
	}
	void execute() override {
		*m_startTime = (float)glfwGetTime();
	}
	float* m_startTime;
};

CountingSheep::CountingSheep(std::string mapName) : Map(mapName)
{
	camera->set_camera_pos(glm::vec3(130.0f, 170.0f, 270.0f));
	camera->set_camera_front(glm::vec3(120.0f, 0.0f, 0.0f) - camera->get_camera_pos());
	startTime = (float)glfwGetTime();
}

void CountingSheep::update(float deltaTime)
{
	if ((float)glfwGetTime() < startTime + 10)
	{
		int xzPos = ErlMath::generate_random(0, 1);
		int posNeg = ErlMath::generate_random(0, 1);
		int randSheep = ErlMath::generate_random(0, numSheep - 1);
		int move = ErlMath::generate_random(0, 5);

		if (xzPos == 0)
		{
			if (posNeg == 0)
			{
				sheep[randSheep]->Position.x += move;
			}
			else {
				sheep[randSheep]->Position.x -= move;

			}
		}
		else {
			if (posNeg == 0)
			{
				sheep[randSheep]->Position.z += move;
			}
			else {
				sheep[randSheep]->Position.z -= move;
			}
		}
		UIManager::get_text_element(0)->text = "Count: " + std::to_string(counts[0]);
	}
	else 
	{
		//will keep adding..
		if (counts[0] == numSheep)
		{
			UIManager::add_text_element("actual amount: " + std::to_string(numSheep) + ", Your guess: " + std::to_string(counts[0]));
		}
		else {
			UIManager::add_text_element("actual amount: " + std::to_string(numSheep));
		}
	}
}

void CountingSheep::load_skinned_objects()
{
	numSheep = ErlMath::generate_random(30, 100);
	
	for (int i = 0; i < numSheep; i++)
	{
		GameObject* shoop = new GameObject(7, AssetManager::get_model(7), glm::vec3(ErlMath::generate_random(0, 200), 0.0f, ErlMath::generate_random(-100, 100)), glm::vec3(0.1f), glm::vec3(-90.0f, 0.0f, -90.0f));
		sheep.push_back(shoop);
		Renderer::add_render_object(shoop);
	}

	counts[0] = 0;
	UIManager::add_text_element("text");
}

void CountingSheep::set_controls(float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		InputManager::set_gamepad_binding({ i, GLFW_GAMEPAD_BUTTON_DPAD_UP }, new AddSubtractCountCommand(counts[0], true));
		InputManager::set_gamepad_binding({ i, GLFW_GAMEPAD_BUTTON_DPAD_DOWN }, new AddSubtractCountCommand(counts[0], false));
		InputManager::set_gamepad_binding({ i, GLFW_GAMEPAD_BUTTON_DPAD_LEFT }, new Restart(&startTime));
	}
}

void CountingSheep::draw(float deltaTime)
{
	Renderer::render(camera);
	UIManager::draw();
}