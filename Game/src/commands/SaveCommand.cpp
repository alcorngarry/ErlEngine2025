#include"SaveCommand.h"

SaveCommand::SaveCommand(Map* map) : map(map) {}

void SaveCommand::execute(float deltaTime) {
	map->save();
	std::cout << "saved current map" << std::endl;
}