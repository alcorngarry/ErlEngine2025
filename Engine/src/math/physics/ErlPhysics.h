#ifndef ERL_PHYSICS_H
#define ERL_PHYSICS_H
#include<algorithm>
#include<vector>
#include<glm/glm.hpp>
#include<common/Camera.h>
#include<game_objects/GameObject.h>

namespace ErlPhysics {
	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
		float length;
	};

	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
	};

	struct CollisionResult {
		bool hit;
		float timeOfImpact;
		glm::vec3 normal;
		glm::vec3 contactPoint;
		GameObject* hitObject;
	};

	struct PhysicsObject {
		AABB aabb;
		glm::vec3 velocity;
		glm::vec3 position;
	};

	ErlPhysics::Ray* cast_ray_from_mouse(Camera* camera, float xpos, float ypos);
	void add_physics_object(GameObject* object);
	ErlPhysics::Ray* cast_ray_from_screen(Camera* camera);
	int check_collision(Ray* ray, std::vector<GameObject*> entities);
	int check_aabb_collision(glm::vec3 targetMin, glm::vec3 targetMax, std::vector<GameObject*> entities);
	float check_floor_collision(glm::vec3 position, std::vector<GameObject*> entities);
	AABB calculate_minkowski_difference(glm::vec3 minA, glm::vec3 vec3, glm::vec3 minB, glm::vec3 maxB);
};
#endif // !ERL_PHYSICS_H