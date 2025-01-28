#ifndef PLAYER_H
#define PLAYER_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<random>
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
		int moves = 0;
		unsigned int groats = 0;
		unsigned int selectedCardIndex = 0;
		bool inMotion = false;

		Player(unsigned int playerId, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, PlayerControls controls);
		void move_player(std::vector<GameObject*> boardSpaces);
		void start_move(float startTime, int moves);
		unsigned int get_board_position();
		void update(float deltaTime) override;
		void init_deck();
		unsigned int* get_cards();
		unsigned int get_selected_card_index();
		void add_groats(unsigned int groats);
		void remove_groats(unsigned int groats);
	private:
		int boardPosition = 0;
		float startTime;
		unsigned int playerId = -1;
		unsigned int cards[5];
		PlayerControls m_controls;
		void set_controls(PlayerControls controls);
		unsigned int draw_card();
};
#endif // !PLAYER_H
