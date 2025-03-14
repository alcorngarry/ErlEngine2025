#include"ToggleMenuCommand.h"

ToggleMenuCommand::ToggleMenuCommand(Game* game) : game(game) {}

void ToggleMenuCommand::execute() {
    game->State = (game->State == DEBUG_MENU) ? MAP_UPDATE : DEBUG_MENU;
}