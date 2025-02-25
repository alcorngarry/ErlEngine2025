#ifndef SKINNED_GAME_OBJECT_H
#define SKINNED_GAME_OBJECT_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"game_objects/GameObject.h"
#include"game_objects/animation/Animation.h"
#include"game_objects/animation/Animator.h"

class SkinnedGameObject : public GameObject {
	public:
		std::vector<glm::mat4>* transforms;
		Animator* mator;
		Animation* mation;

		SkinnedGameObject(uint8_t id, char* animationPath, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		void update(float deltaTime) override;
};
#endif // !SKINNED_GAME_OBJECT_H
