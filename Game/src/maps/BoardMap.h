#ifndef BOARD_MAP_H
#define BOARD_MAP_H
#include"common/Map.h"
#include"game_objects/Player.h"
#include"commands/SelectCardLeftCommand.h"
#include"commands/SelectCardRightCommand.h"
#include"commands/SelectCardCommand.h"

class BoardMap : public Map {
	public:
		PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };

		enum BoardState {
			PLAYER_TURN,
			PLAYER_MOVE
		};
		std::vector<Player*> players;

		BoardMap(std::string mapName);
		std::vector<GameObject*> get_board_objects();
		void update(float deltaTime) override;
		void draw(float deltaTime) override;
	private:
		int currentPlayer = 0;
		void load_skinned_objects() override;
		void set_controls(float deltaTime) override;
		void process_board_space(unsigned int boardId);
		void display_cards();
};

#endif // !BOARD_MAP_H
