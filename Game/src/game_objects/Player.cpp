#include"Player.h"

Player::Player(uint8_t playerId, Model* model, Camera* cam, glm::vec3 pos) : GameObject(99, model, pos, glm::vec3(5.0f), glm::vec3(0.0f), true)
{ 
	Position = pos + glm::vec3(0.0f, playerHeight, 0.0f);
	camera = cam;
	camera->set_camera_pos(Position);
}

void Player::move_player()
{		
}

void Player::update(float deltaTime)
{
	Position = camera->get_camera_pos();
	if (jumping)
	{  
		jumpVelocity += -90.0f * deltaTime;
		Position = velocity += glm::vec3(0.0f, jumpVelocity * deltaTime, 0.0f);

		if (Position.y <= floorHeight + playerHeight)
		{
			jumpVelocity = 0.0f;
			velocity = glm::vec3(Position.x, floorHeight + playerHeight, Position.z);
			jumping = false;
		}
	}
	else {
		if (!floorHeight > playerHeight / 2)
		{
			velocity = glm::vec3(Position.x, floorHeight + playerHeight, Position.z);
		}
	}
	set_position(velocity);
}

glm::vec3 Player::get_aabb_max()
{
	//is in world space
	return glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::vec3 Player::get_aabb_min()
{
	//is in world space
	return glm::vec3(1.0f, 1.0f, 1.0f);
}

void Player::set_controls(PlayerControls controls)
{
	m_controls = controls;
}

void Player::set_position(glm::vec3 pos)
{
	set_model_matrix(Position, Rotation, Size);
	camera->set_camera_pos(pos);
}