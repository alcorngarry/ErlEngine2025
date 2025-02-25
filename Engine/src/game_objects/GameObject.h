#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include<glad/glad.h>
#include<glm/glm.hpp>
#include"Model.h"

class GameObject {
	public:
		glm::vec3 Position,Size,Rotation;
		glm::mat4 ModelMatrix;
		Model* GameModel;
		uint8_t id;
		
		GameObject(uint8_t id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		
		virtual void update(float deltaTime);
		glm::vec3 get_aabb_min() const;
		glm::vec3 get_aabb_max() const;
	private:
		glm::vec3 local_to_world(const glm::vec3& localPos) const;
		void set_model_matrix(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);
};

#endif // !GAME_OBJECT_H
