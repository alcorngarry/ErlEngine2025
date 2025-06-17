#include"ToggleConsoleCommand.h"

ToggleConsoleCommand::ToggleConsoleCommand() {}

void ToggleConsoleCommand::execute(float deltaTime) {
	UIManager::toggle_window("console");
	InputManager::set_text_input_control();
;}