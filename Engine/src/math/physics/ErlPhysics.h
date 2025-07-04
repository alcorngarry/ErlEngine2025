#ifndef ERL_PHYSICS_H
#define ERL_PHYSICS_H
#include<algorithm>
#include<vector>
#include<glm/glm.hpp>
#include<common/Camera.h>
#include<game_objects/GameObject.h>
#include<set>
#include<game_objects/Player.h>
#include<game_objects/NPC.h>

namespace ErlPhysics {
	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
		float length;
	};

	struct PhysicsObject {
		GameObject* object;
		bool hit = false;
	};

	struct PlayerPhysicsObject {
		Player* player;
		bool hit = false;
	};

	struct SweptCollisionResult {
		float collisionTime;
		glm::vec3 normal;
		uint16_t objId;
	};

	struct RayCollisionResult {
		int index;
		glm::vec3 collisionPos;
	};

	void update(float deltaTime);
	ErlPhysics::Ray* cast_ray_from_mouse(Camera* camera, float xpos, float ypos);
	void add_physics_object(GameObject* object);
	void add_physics_camera(Camera* camera);
	void remove_physics_object(uint16_t id);
	void add_player_physics_object(Player* player);
	ErlPhysics::Ray* cast_ray_from_screen(Camera* camera, float distance);
	ErlPhysics::Ray* cast_ray_from_player(Player* player, float distance);
	RayCollisionResult* check_collision(Ray* ray);
	std::set<uint16_t> get_collided_objects();
	float check_floor_collision(Player* player);
	float check_floor_collision(NPC* npc);
	void add_ray(ErlPhysics::Ray* ray);
	std::vector<ErlPhysics::Ray*> get_rays();
	void remove_ray_object(int index);
	void handle_collision(GameObject* a, GameObject* b, float deltaTime);
	void swept_aabb_collision(GameObject* player, float deltaTime);
	void camera_collision(Camera* camera, float deltaTime);
};
#endif // !ERL_PHYSICS_H