#include"SelectEntityCommand.h"

SelectEntityCommand::SelectEntityCommand(float xpos, float ypos, bool isDeselect) : xpos(xpos), ypos(ypos), isDeselect(isDeselect) {}

void SelectEntityCommand::execute() {
	if (isDeselect)
	{
		Renderer::deselect_index();
	}
	else {
		if (Renderer::get_selected_index() == -1)
		{
			Renderer::select_entity(xpos, ypos);
		}
	}
}