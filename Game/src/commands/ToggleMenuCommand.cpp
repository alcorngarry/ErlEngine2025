#include"ToggleMenuCommand.h"

ToggleMenuCommand::ToggleMenuCommand(Game* game) : game(game) {}

void ToggleMenuCommand::execute(float deltaTime) {
    GameState before = game->State;
    game->State = (before == DEBUG_MENU) ? MAP_UPDATE : DEBUG_MENU;
    game->update_controls(before, game->State);
}