#include"Player.h"

Player::Player(unsigned int playerId, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, PlayerControls controls) : SkinnedGameObject(playerId, (char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae", characterModel, pos, size, rotation)
{ 
	this->m_controls = controls;
	state = INACTIVE;
}

void Player::move_player(std::vector<GameObject*> boardSpaces)
{		
	int nextPos = boardPosition == boardSpaces.size() - 1 ? 0 : boardPosition + 1;
	float delta = 1.0f;

	glm::vec3 buffer(0.0f, 1.0f, 0.0f);

  	if (boardPosition != moves)
	{
		if ((float)glfwGetTime() < startTime + delta)
		{
			//algorithm to travel distance given start and delta time.
			//A + (B -A)(t - t0)/deltaT
			glm::vec3 objectPos = boardSpaces[boardPosition]->Position;
			glm::vec3 direction = (boardSpaces[nextPos]->Position - boardSpaces[boardPosition]->Position);
			glm::vec3 currPos = objectPos + direction * ((float)glfwGetTime() - startTime) / delta;
			
			Rotation = glm::vec3(0.0f, glm::degrees(std::atan2(direction.x, direction.z)), 0.0f);
			Position = currPos + buffer;
		}
		else {
			boardPosition = nextPos;
			startTime = (float)glfwGetTime();

			Position = boardSpaces[boardPosition]->Position + buffer;
		}
	}
	else {
		Position = boardSpaces[boardPosition]->Position + buffer;
		mator->reset_animation();
		transforms = mator->get_final_bone_matrices();
		inMotion = false;

		cards[selectedCardIndex] = draw_card();
		selectedCardIndex = 0; //reset to start on card 1
	}

	//fix this
	if (moves >= boardSpaces.size())
	{
		moves -= boardSpaces.size();
		//do an actual implementation;
	}
}

void Player::update(float deltaTime)
{
	transforms = mator->get_final_bone_matrices();
	mator->update_animation(deltaTime);
}

unsigned int Player::get_board_position()
{
	return boardPosition;
}

void Player::start_move(float startTime, int moves)
{
	this->startTime = startTime;
	// can set total moves if needed here.
	this->moves = boardPosition + moves;
}

void Player::init_deck()
{
	for (int i = 0; i < 5; i++)
	{
		cards[i] = draw_card();
		std::cout << "card " << i << ": " << cards[i] << std::endl;
	}
}

unsigned int Player::draw_card()
{
	return ErlMath::generate_random(1, 13);
}

unsigned int* Player::get_cards()
{
	return cards;
}

unsigned int Player::get_selected_card_index()
{
	return selectedCardIndex;
}

void Player::add_groats(unsigned int groats)
{
	this->groats += groats;
}

void Player::remove_groats(unsigned int groats)
{
	if (this->groats >= groats)
	{
		this->groats -= groats;
	}
}

void Player::set_controls(PlayerControls controls)
{
	m_controls = controls;
}