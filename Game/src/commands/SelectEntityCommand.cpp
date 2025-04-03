#include"SelectEntityCommand.h"

SelectEntityCommand::SelectEntityCommand(bool isDeselect) : isDeselect(isDeselect) {}

void SelectEntityCommand::execute() {
	if (isDeselect)
	{
		Renderer::deselect_index();
	}
	else {
		if (Renderer::get_selected_index() == -1)
		{
			Renderer::select_entity(InputManager::get_xpos(), InputManager::get_ypos());
		}
	}
}