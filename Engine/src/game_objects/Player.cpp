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
	Position += Velocity * deltaTime;

	camera->set_camera_pos(Position);
	camera->set_camera_up(glm::vec3(0.0f, Position.y, 0.0f));
	set_model_matrix(Position, Rotation, Size);
}

void Player::update_movement(float deltaTime)
{
	if (fmovePrev == fmove) fmove = 0.0f, fmovePrev = 0.0f;
	if (smovePrev == smove) smove = 0.0f, smovePrev = 0.0f;
	friction(deltaTime);

	wishSpeed = 0.0f;

	wishVelocity = fmove * glm::normalize(glm::vec3(camera->get_camera_front().x, 0.0f, camera->get_camera_front().z)) +
		smove * glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up()));

	if (wishVelocity != glm::vec3(0.0f))
	{
		wishDir = glm::normalize(wishVelocity);
		wishSpeed = glm::length(wishVelocity);
	}

	if (wishSpeed > maxSpeed)
	{
		wishVelocity *= maxSpeed / wishSpeed;
		wishSpeed = maxSpeed;
	}

	if (onGround)
	{
		/*if (!floorHeight > playerHeight / 2)
		{
			Velocity += glm::vec3(Velocity.x, floorHeight + playerHeight, Velocity.z);
		}*/
		accelerate(deltaTime);
	}
	else {
		air_accelerate(deltaTime);

		//Velocity.y += -90.0f * deltaTime;
		Velocity.y -= 1.0f * 800.0f * deltaTime;

		if (Position.y < floorHeight + playerHeight)
		{
			Velocity.y = 0.0f;
			Position = glm::vec3(Position.x, floorHeight + playerHeight, Position.z);
			onGround = true;
		}
	}

	fmovePrev = fmove;
	smovePrev = smove;
}

void Player::set_controls(PlayerControls controls)
{
	m_controls = controls;
}

void Player::accelerate(float deltaTime)
{
	float currSpeed = glm::dot(Velocity, wishDir);
	float addSpeed = wishSpeed - currSpeed;
	if (addSpeed <= 0) return;
	float accelSpeed = 10.0f * deltaTime * wishSpeed;
	if (accelSpeed > addSpeed) accelSpeed = addSpeed;

	Velocity += accelSpeed * wishDir;
}

void Player::air_accelerate(float deltaTime)
{
	if (wishSpeed > 30) wishSpeed = 30;
	float currSpeed = glm::dot(Velocity, wishDir);
	float addSpeed = wishSpeed - currSpeed;
	if (addSpeed <= 0) return;

	float accelSpeed = 10.0f * wishSpeed * deltaTime;
	if (accelSpeed > addSpeed) accelSpeed = addSpeed;

	Velocity += accelSpeed * wishDir;
}

void Player::friction(float deltaTime)
{
	float speed = glm::length(Velocity);

	if (speed < 1)
	{
		Velocity.x = 0.0f;
		Velocity.z = 0.0f;
		return;
	}

	float drop = 0;

	if (onGround)
	{
		float control = speed < 100.0f ? 100.0f : speed;
		drop = control * fric * deltaTime;
	}
	

	float newspeed = speed - drop;
	if (newspeed < 0.0f) newspeed = 0.0f;
	newspeed /= speed;
	Velocity *= newspeed;
}