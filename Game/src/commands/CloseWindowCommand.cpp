#include"CloseWindowCommand.h"

CloseWindowCommand::CloseWindowCommand(GLFWwindow* window) : window(window) {}

void CloseWindowCommand::execute(float deltaTime)
{
	glfwSetWindowShouldClose(window, true);
}