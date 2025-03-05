#include"SelectCardCommand.h"

SelectCardCommand::SelectCardCommand(Player* player) : player(player){}

void SelectCardCommand::execute() {
    if (!player->state == IN_MOTION) {
        //player->inMotion = true;
        player->startTime = static_cast<float>(glfwGetTime());
        player->moves = player->get_cards()[player->get_selected_card_index()];
        player->state = IN_MOTION;
        player->travelDistance += player->moves;
        //player->start_move(, );
    }
}