#include"Engine.h"
#include"renderer/Renderer.h" 

int main(int argc, char** argv)
{
	GLFWwindow* window = Engine::start();
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();

		InputManager::update();
	}

	Engine::shut_down(window);
	return 0;
}