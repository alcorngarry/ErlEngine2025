#ifndef ADD_SUBTRACT_COUNT_COMMAND_H
#define ADD_SUBTRACT_COUNT_COMMAND_H
#include"input/Command.h"
#include<GLFW/glfw3.h>
#include<iostream>

class AddSubtractCountCommand : public Command {
public:
    AddSubtractCountCommand(int& count, bool isAdd);
    void execute() override;
private:
    int& count;
    bool isAdd;
};
#endif // !ADD_SUBTRACT_COUNT_COMMAND_H