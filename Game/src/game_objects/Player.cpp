#include"Player.h"

Player::Player(uint8_t playerId, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, PlayerControls controls) : SkinnedGameObject(playerId, (char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae", characterModel, pos, size, rotation)
{ 
	this->m_controls = controls;
	state = INACTIVE;
}

void Player::move_player()
{		
	float delta = 1.0f;

  	if (moves > 0)
	{
		if ((float)glfwGetTime() < startTime + delta)
		{
			//algorithm to travel distance given start and delta time.
			//A + (B -A)(t - t0)/deltaT
			glm::vec3 objectPos = currSpace->Position;
			glm::vec3 direction = currSpace->nextSpace[0]->Position - objectPos;
			glm::vec3 currPos = objectPos + direction * ((float)glfwGetTime() - startTime) / delta;
			
			Rotation = glm::vec3(0.0f, glm::degrees(std::atan2(direction.x, direction.z)), 0.0f);
			Position = currPos;
		}
		else {
			currSpace = currSpace->nextSpace[0];
			startTime = (float)glfwGetTime();
			Position = currSpace->Position;
			moves--;
		}
	}
	else {
		Position = currSpace->Position;
		mator->reset_animation();
		transforms = mator->get_final_bone_matrices();
		state = TURN_COMPLETE;

		cards[selectedCardIndex] = draw_card();
		selectedCardIndex = 0; //reset to start on card 1
	}
	set_model_matrix(Position, Rotation, Size);
}

void Player::update(float deltaTime)
{
	transforms = mator->get_final_bone_matrices();
	mator->update_animation(deltaTime);
}

void Player::init_deck()
{
	for (int i = 0; i < 5; i++)
	{
		cards[i] = draw_card();
		std::cout << "card " << i << ": " << cards[i] << std::endl;
	}
}

uint8_t Player::draw_card()
{
	return ErlMath::generate_random(1, 13);
}

uint8_t* Player::get_cards()
{
	return cards;
}

uint8_t Player::get_selected_card_index()
{
	return selectedCardIndex;
}

void Player::add_groats(uint8_t groats)
{
	this->groats += groats;
}

void Player::remove_groats(uint8_t groats)
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