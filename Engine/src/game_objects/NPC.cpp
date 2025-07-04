#include"NPC.h"

NPC::NPC(uint16_t npcId, Model* model, glm::vec3 pos)
	: GameObject(npcId, model, pos, glm::vec3(5.0f), glm::vec3(0.0f), true)
{
	Position = pos;
	npcActions["seek"] = [this] (Player* player, float deltaTime) {
		if (glm::distance(player->Position, Position) < 100.0f)
		{
			wishVelocity = glm::normalize(player->Position - Position) * 120.0f;
			update_movement(deltaTime);
		} else {
			wishSpeed = 0.0f;
			wishVelocity = glm::vec3(0.0f);
			Velocity = glm::vec3(0.0f);
		}
	};

	npcActions["attack"] = [this](Player* player, float deltaTime) {
		if (glm::distance(player->Position, Position) < 5.0f)
		{
			player->health -= 10.0f;
		}
	};
}

void NPC::update(float deltaTime)
{
	Position += Velocity * deltaTime;
	set_model_matrix(Position, Rotation, Size);

	for (const auto& action : npcActions)
	{
		if (players.size() > 0)
		{
			action.second(players[0], deltaTime);
		}
	}
}

void NPC::update_movement(float deltaTime)
{
	//dynamic follow camera move to camera class (pass in both players)
	friction(deltaTime);

	wishSpeed = 0.0f;

	if (wishVelocity != glm::vec3(0.0f))
	{
		wishDir = glm::normalize(wishVelocity);
		wishSpeed = glm::length(wishVelocity);
		Rotation.y = glm::degrees(atan2(wishDir.x, wishDir.z));
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
		Velocity.y -= 1.0f * 800.0f * deltaTime;

		if (Position.y < floorHeight)
		{
			Velocity.y = 0.0f;
			Position = glm::vec3(Position.x, floorHeight, Position.z);
			onGround = true;
		}
	}
}

void NPC::accelerate(float deltaTime)
{
	float currSpeed = glm::dot(Velocity, wishDir);
	float addSpeed = wishSpeed - currSpeed;
	if (addSpeed <= 0) return;
	float accelSpeed = 10.0f * deltaTime * wishSpeed;
	if (accelSpeed > addSpeed) accelSpeed = addSpeed;

	Velocity += accelSpeed * wishDir;
}

void NPC::air_accelerate(float deltaTime)
{
	if (wishSpeed > 30) wishSpeed = 30;
	float currSpeed = glm::dot(Velocity, wishDir);
	float addSpeed = wishSpeed - currSpeed;
	if (addSpeed <= 0) return;

	float accelSpeed = 10.0f * wishSpeed * deltaTime;
	if (accelSpeed > addSpeed) accelSpeed = addSpeed;

	Velocity += accelSpeed * wishDir;
}

void NPC::friction(float deltaTime)
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