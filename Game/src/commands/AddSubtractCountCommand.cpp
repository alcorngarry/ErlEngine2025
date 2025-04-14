#include"AddSubtractCountCommand.h"

AddSubtractCountCommand::AddSubtractCountCommand(int& count, bool isAdd) : count(count), isAdd(isAdd) {}

void AddSubtractCountCommand::execute(float deltaTime)
{
	if (isAdd)
	{
		count++;
	}
	else {
		count--;
	}
	std::cout << count << std::endl;
}