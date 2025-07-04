#include"GameObject.h"

int GameObject::idCounter = 0;

// lets do this, goal make a script that if both players press button it opens a door.
GameObject::GameObject(uint16_t assetId, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, bool isRendered) : assetId(assetId), GameModel(model), Position(pos), Size(size), Rotation(rotation), isRendered(isRendered)
{
	instanceId = idCounter++;
	Velocity = glm::vec3(0.0f);
	set_model_matrix(Position, Rotation, Size);
}

void GameObject::update(float deltaTime)
{
	for (const auto& action : actions)
	{
		action.second(this, deltaTime);
	}
}

void GameObject::on_collision(GameObject* object, float deltaTime)
{
	auto it = onCollisionActions.find(object->assetId);
	if (it != onCollisionActions.end()) {
		it->second(object, deltaTime);
	}
	else {
		//std::cerr << "No collision handler for assetId: " << object->assetId << "\n";
	}
}

void GameObject::on_ray_collision(GameObject* object, float deltaTime)
{
	auto it = onRayCollisionActions.find(object->assetId);
	if (it != onRayCollisionActions.end()) {
		it->second(object, deltaTime);
	}
	else {
		//std::cerr << "No collision handler for assetId: " << object->assetId << "\n";
	}
}

glm::vec3 GameObject::get_aabb_max()
{
	return local_to_world(GameModel->getMaxAABB());
}

glm::vec3 GameObject::get_aabb_min()
{
	return local_to_world(GameModel->getMinAABB());
}

glm::vec3 GameObject::local_to_world(const glm::vec3& localPos)
{
	AABBMatrix = glm::mat4(1.0f);
	AABBMatrix = glm::translate(AABBMatrix, Position);
	AABBMatrix = glm::scale(AABBMatrix, Size);

	glm::vec4 worldPos = AABBMatrix * glm::vec4(localPos, 1.0f);
	return glm::vec3(worldPos);
}

void GameObject::set_model_matrix(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);

	ModelMatrix = modelMatrix;
}
