#ifndef ENGINE_H
#define ENGINE_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"input/InputManager.h"
#include"renderer/Renderer.h"

namespace Engine {
	GLFWwindow* start(float windowWidth, float windowHeight);
	void shut_down(GLFWwindow* window);
};

#endif // !ENGINE_H