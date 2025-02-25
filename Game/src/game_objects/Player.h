#ifndef PLAYER_H
#define PLAYER_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"math/ErlMath.h"
#include"game_objects/SkinnedGameObject.h"
#include"game_objects/animation/Animation.h"
#include"game_objects/animation/Animator.h"

struct PlayerControls {
	unsigned int move_up;
	unsigned int move_down;
	unsigned int select_card;
	unsigned int left;
	unsigned int right;
};

enum State {
	ACTIVE,
	INACTIVE
};

class Player : public SkinnedGameObject {
	public:
		glm::vec3 velocity = glm::vec3(0.0f);
		State state;
		uint8_t moves = 0;
		uint8_t groats = 0;
		uint8_t selectedCardIndex = 0;
		bool inMotion = false;

		Player(uint8_t playerId, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, PlayerControls controls);
		void move_player(std::vector<GameObject*> boardSpaces);
		void start_move(float startTime, int moves);
		uint8_t get_board_position();
		void update(float deltaTime) override;
		void init_deck();
		uint8_t* get_cards();
		uint8_t get_selected_card_index();
		void add_groats(uint8_t groats);
		void remove_groats(uint8_t groats);
	private:
		uint8_t boardPosition = 0;
		float startTime;
		uint8_t cards[5];
		PlayerControls m_controls;
		void set_controls(PlayerControls controls);
		uint8_t draw_card();
};
#endif // !PLAYER_H
