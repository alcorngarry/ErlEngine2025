#include"SkinnedGameObject.h"

SkinnedGameObject::SkinnedGameObject(uint8_t id, char* animationPath, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameObject(id, characterModel, pos, size, rotation, true)
{
	mation = new Animation(animationPath, characterModel);
	mator = new Animator(mation);
	transforms = mator->get_final_bone_matrices();
}

void SkinnedGameObject::update(float deltaTime)
{
	transforms = mator->get_final_bone_matrices();
	mator->update_animation(deltaTime);
}