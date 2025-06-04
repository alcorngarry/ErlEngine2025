#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include "networking/Server.h"
#include "Engine.h"
#include "Game.h"
#include "commands/ToggleMenuCommand.h"
#include "commands/CloseWindowCommand.h"
#include <math/physics/ErlPhysics.h>

int main(int argc, char** argv)
{
	float window_width = 1280;
	float window_height = 768;
    // Start the server in a separate thread
	//std::thread serverThread(Server::start_server);
	//Engine::set_full_screen(true);
	GLFWwindow* window = Engine::start(window_width, window_height);
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	Game* game = new Game(window_width, window_height);
	game->init();

	//move these to the engine probably
	InputManager::set_key_binding(GLFW_KEY_F9, new SaveCommand(game->Maps[game->level]));
	InputManager::set_key_binding(GLFW_KEY_M, new ToggleMenuCommand(game));
	InputManager::set_key_binding(GLFW_KEY_ESCAPE, new CloseWindowCommand(window));
	//move
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	while (!glfwWindowShouldClose(window))
	{
		InputManager::update(deltaTime);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ErlPhysics::update(deltaTime);
		game->update(deltaTime);

		//do you need this?
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		game->render(deltaTime);

		//render second screen
		/*glfwGetFramebufferSize(window, &width, &height);
		glViewport(width / 2, 0, width / 2, height);

		game->render(deltaTime);*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Engine::shut_down(window);
	return 0;
}