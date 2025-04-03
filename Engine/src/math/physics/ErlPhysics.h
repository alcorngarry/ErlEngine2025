#ifndef ERL_PHYSICS_H
#define ERL_PHYSICS_H
#include<renderer/Renderer.h>
#include<algorithm>

namespace ErlPhysics{
	void cast_ray_from_mouse(Camera* camera, float xpos, float ypos);
	void cast_ray_from_screen(Camera* camera);
	int check_collision(Renderer::Ray* ray, std::vector<GameObject*> entities);
};
#endif // !ERL_PHYSICS_H