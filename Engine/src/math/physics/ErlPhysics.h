#ifndef ERL_PHYSICS_H
#define ERL_PHYSICS_H
#include<algorithm>
#include<vector>
#include<glm/glm.hpp>
#include<common/Camera.h>
#include<game_objects/GameObject.h>
#include<set>

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

	struct PhysicsObject {
		GameObject* object;
		bool hit = false;
	};

	void update(float deltaTime);
	ErlPhysics::Ray* cast_ray_from_mouse(Camera* camera, float xpos, float ypos);
	void add_physics_object(GameObject* object);
	void add_player_physics_object(GameObject* object);
	ErlPhysics::Ray* cast_ray_from_screen(Camera* camera);
	int check_collision(Ray* ray, std::vector<GameObject*> entities);
	void check_aabb_collision();
	std::set<uint16_t> get_collided_objects();
	float check_floor_collision(glm::vec3 position, std::vector<GameObject*> entities);
	AABB calculate_minkowski_difference(glm::vec3 minA, glm::vec3 vec3, glm::vec3 minB, glm::vec3 maxB);
	void add_ray(ErlPhysics::Ray* ray);
	std::vector<ErlPhysics::Ray*> get_rays();
	void remove_ray_object(int index);
};
#endif // !ERL_PHYSICS_H