#ifndef PLAYER_H
#define PLAYER_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"math/ErlMath.h"
#include"game_objects/GameObject.h"
#include"common/Camera.h"

struct PlayerControls {
};

class Player : public GameObject {
	public:
		Camera* camera;
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec2 viewAngles;

		float jumpForce = 50.0f;
		float friction = 8.0f;
		float moveSpeed = 1000.0f;
		float floorHeight = 0.0f;
		bool onGround = true;

		Player(uint8_t playerId, Model* model, Camera* cam, glm::vec3 pos);
		void move_player();
		void update(float deltaTime);
	private:
		PlayerControls m_controls;
		float playerHeight = 5.0f;
		void set_controls(PlayerControls controls);
};
#endif // !PLAYER_H
