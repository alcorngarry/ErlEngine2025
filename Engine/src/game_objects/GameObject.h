#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include<glad/glad.h>
#include<glm/glm.hpp>
#include"Model.h"

class GameObject {
	public:
		glm::vec3 Position,Size,Rotation;

		unsigned int id;
		Model* GameModel;
		GameObject(unsigned int id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		GameObject();
		virtual void update(float deltaTime);
		glm::vec3 get_aabb_min() const;
		glm::vec3 get_aabb_max() const;

		glm::vec3 get_position() const;
		glm::vec3 get_size() const;
		glm::vec3 get_rotation() const;
		void set_position(glm::vec3 pos);
		void set_size(glm::vec3 size);
		void set_rotation(glm::vec3 rotation);

	private:
		glm::vec3 local_to_world(const glm::vec3& localPos) const;
};

#endif // !GAME_OBJECT_H
