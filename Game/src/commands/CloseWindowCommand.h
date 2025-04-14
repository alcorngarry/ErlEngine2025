#ifndef CLOSE_WINDOW_COMMAND_H
#define CLOSE_WINDOW_COMMAND_H
#include"input/Command.h"
#include<GLFW/glfw3.h>

class CloseWindowCommand : public Command {
public:
    CloseWindowCommand(GLFWwindow* window);
    void execute(float deltaTime) override;
private:
    GLFWwindow* window;
};
#endif // !CLOSE_WINDOW_COMMAND_H