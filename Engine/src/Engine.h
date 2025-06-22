#ifndef ENGINE_H
#define ENGINE_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"input/InputManager.h"
#include"common/DebugMenu.h"
#include"renderer/Renderer.h"

namespace Engine {
	GLFWwindow* start(float windowWidth, float windowHeight);
	void shut_down(GLFWwindow* window);
	void toggle_full_screen();
};

#endif // !ENGINE_H