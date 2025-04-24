#include"Player.h"

Player::Player(uint8_t playerId, Model* model, Camera* cam, glm::vec3 pos) : GameObject(99, model, pos, glm::vec3(5.0f), glm::vec3(0.0f), true)
{ 
	Position = pos + glm::vec3(0.0f, playerHeight + 3, 0.0f);
	camera = cam;
	camera->set_camera_pos(Position);
}

void Player::move_player()
{		
}

void Player::update(float deltaTime)
{
	if (!onGround)
	{  
		Velocity.y += -90.0f * deltaTime;

		if (Position.y < floorHeight + playerHeight)
		{
			Velocity.y = 0.0f;
			Position = glm::vec3(Position.x, floorHeight + playerHeight, Position.z);
			onGround = true;
		}
	}
	else {
		if (!floorHeight > playerHeight / 2)
		{
			Velocity += glm::vec3(Velocity.x, floorHeight + playerHeight, Velocity.z);
		}
	}
	
	
	Velocity.x *= (1.0f - 8.0f * deltaTime);
	Velocity.z *= (1.0f - 8.0f * deltaTime);

	/*if (Velocity.x < 1.0f) Velocity.x = 0.0f;
	if (Velocity.z < 1.0f) Velocity.z = 0.0f;*/


	Position += Velocity * deltaTime;
	camera->set_camera_pos(Position);
	camera->set_camera_up(glm::vec3(0.0f,Position.y, 0.0f));
	set_model_matrix(Position, Rotation, Size);
}

void Player::set_controls(PlayerControls controls)
{
	m_controls = controls;
}