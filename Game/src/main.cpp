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
	float window_height = 720;

    // Start the server in a separate thread
	//std::thread serverThread(Server::start_server);

	GLFWwindow* window = Engine::start(window_width, window_height);
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	Game* game = new Game(window);
	game->init();

	SaveCommand sc(game->Maps[game->level]);
	ToggleMenuCommand tm(game);
	CloseWindowCommand cw(window);

	//move these to the engine probably
	InputManager::set_key_binding(GLFW_KEY_F9, [sc](float dt) mutable { sc.execute(dt); });
	InputManager::set_key_binding(GLFW_KEY_M, [tm](float dt) mutable { tm.execute(dt); });
	InputManager::set_key_binding(GLFW_KEY_ESCAPE, [cw](float dt) mutable  { cw.execute(dt); });
	InputManager::set_key_binding(GLFW_KEY_F11, [](float dt) { Engine::toggle_full_screen(); });

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

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->render(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Engine::shut_down(window);
	return 0;
}