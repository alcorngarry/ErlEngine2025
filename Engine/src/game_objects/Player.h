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
		glm::vec3 wishVelocity = glm::vec3(0.0f);
		glm::vec3 wishDir = glm::vec3(0.0f);
		float wishSpeed = 0.0f;
		glm::vec2 viewAngles;

		float fmove = 0.0f;
		float smove = 0.0f;
		//float maxSpeed = 320.0f;
		float maxSpeed = 160.0f;


		float fmovePrev = 0.0f;
		float smovePrev = 0.0f;
		float playerHeight = 5.0f;

		float jumpForce = 270.0f;
		float fric = 4.0f;
		float floorHeight = 0.0f;
		bool onGround = true;

		Player(uint8_t playerId, Model* model, Camera* cam, glm::vec3 pos);
		void move_player();
		void update(float deltaTime);
		void update_movement(float deltaTime);
	private:
		PlayerControls m_controls;
		void set_controls(PlayerControls controls);
		void accelerate(float deltaTime);
		void air_accelerate(float deltaTime);
		void friction(float deltaTime);
		glm::vec3 get_direction();
};
#endif // !PLAYER_H
