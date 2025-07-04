#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<functional>
#include"Model.h"

class GameObject {
	public:
		std::map<std::string, std::function<void(GameObject*, float)>> actions;
		// the id is the id of the object
		std::map<uint16_t, std::function<void(GameObject*, float)>> onCollisionActions;
		std::map<uint16_t, std::function<void(GameObject*, float)>> onRayCollisionActions;

		Model* GameModel;
		glm::vec3 Position, Size, Rotation, Velocity, Acceleration;
		glm::mat4 ModelMatrix;
		glm::mat4 AABBMatrix;
		static int idCounter;
		uint16_t assetId;
		uint16_t instanceId;
		bool isRendered;
		
		GameObject(uint16_t assetId, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, bool isRendered);
		virtual void update(float deltaTime);
		void on_collision(GameObject* object, float deltaTime);
		void on_ray_collision(GameObject* object, float deltaTime);
		glm::vec3 get_aabb_min();
		glm::vec3 get_aabb_max();
	protected:
		void set_model_matrix(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);
	private:
		glm::vec3 local_to_world(const glm::vec3& localPos);
};

#endif // !GAME_OBJECT_H
