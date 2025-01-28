#include"Engine.h"
#include"Game.h"

int main(int argc, char** argv)
{
	GLFWwindow* window = Engine::start();
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	Game* game = new Game(window);
	game->init();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game->update(deltaTime);

		//do you need this?
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->render(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();

		InputManager::update();
	}

	Engine::shut_down(window);
	return 0;
}