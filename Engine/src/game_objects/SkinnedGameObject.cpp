#include"SkinnedGameObject.h"

SkinnedGameObject::SkinnedGameObject(unsigned int id, char* animationPath, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameObject(id, characterModel, pos, size, rotation)
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