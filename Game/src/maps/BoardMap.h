#ifndef BOARD_MAP_H
#define BOARD_MAP_H
#include"common/Map.h"
#include"game_objects/Player.h"
#include"game_objects/board_space/BoardSpace.h"
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
		std::vector<BoardSpace*> boardSpaces;

		BoardMap(std::string mapName);
		void update(float deltaTime) override;
		void draw(float deltaTime) override;
		void load() override;
	protected:
		void save() override;
	private:
		int currentPlayer = 0;
		void load_skinned_objects() override;
		void set_controls(float deltaTime) override;
		void process_board_space(uint8_t boardId);
		void display_cards();
		void init_board_spaces();
		void update_camera_position(float deltaTime);
		void write_board_spaces();
		void read_board_spaces() override;
		BoardSpace* read_board_space();
};

#endif // !BOARD_MAP_H
