#ifndef NPC_H
#define NPC_H
#include"game_objects/GameObject.h"
#include"game_objects/Player.h"
#include"common/Camera.h"

class NPC : public GameObject {
public:
	std::vector<Player*> players;
	std::map<std::string, std::function<void(Player*, float)>> npcActions;

	glm::vec3 wishVelocity = glm::vec3(0.0f);
	glm::vec3 wishDir = glm::vec3(0.0f);
	float wishSpeed = 0.0f;

	float maxSpeed = 160.0f;
	float jumpForce = 270.0f;
	float fric = 10.0f;
	float floorHeight = 0.0f;
	bool onGround = true;

	NPC(uint16_t npcId, Model* model, glm::vec3 pos);
	void update(float deltaTime);
	void update_movement(float deltaTime);
	void accelerate(float deltaTime);
	void air_accelerate(float deltaTime);
	void friction(float deltaTime);
private:
	float health = 100.0f;
};
#endif // !NPC_H