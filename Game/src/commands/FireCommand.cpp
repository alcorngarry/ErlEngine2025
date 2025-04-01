#include"FireCommand.h"

FireCommand::FireCommand(){}

void FireCommand::execute() 
{
	Renderer::cast_ray(InputManager::get_xpos(), InputManager::get_ypos());
}