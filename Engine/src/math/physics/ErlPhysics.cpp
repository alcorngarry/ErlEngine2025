#include"ErlPhysics.h"

std::map<uint16_t, ErlPhysics::PhysicsObject*> physObjects;
std::vector<ErlPhysics::PhysicsObject*> playerPhysObjects;
std::vector<ErlPhysics::Ray*> rays;

void ErlPhysics::update(float deltaTime)
{
    check_aabb_collision();
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_mouse(Camera* camera, float xpos, float ypos)
{
	float NDC_X = ((int)xpos * (2.0f / camera->m_windowWidth)) - 1;
	float NDC_Y = -((int)ypos * (2.0f / camera->m_windowHeight)) + 1;
	float near_plane_height = glm::tan(45 / 2.0f) * 0.1f;
	float aspect_ratio = camera->m_windowWidth / camera->m_windowHeight;

	float X_3D = NDC_X * near_plane_height * aspect_ratio;
	float Y_3D = NDC_Y * near_plane_height;

	glm::vec3 near_plane_point(X_3D, Y_3D, -0.1f);
	near_plane_point = glm::inverse(camera->get_view_matrix()) * glm::vec4(near_plane_point, 1.0f);
	glm::vec3 cam_dir_vec = near_plane_point - camera->get_camera_pos();

    return new ErlPhysics::Ray{camera->get_camera_pos(), glm::normalize(cam_dir_vec), 1000.0f};
}

void ErlPhysics::add_physics_object(GameObject* object)
{
    physObjects[object->instanceId] = new ErlPhysics::PhysicsObject{object};
}

void ErlPhysics::add_player_physics_object(GameObject* object)
{
    playerPhysObjects.push_back(new ErlPhysics::PhysicsObject{object});
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_screen(Camera* camera)
{
    return new ErlPhysics::Ray{ camera->get_camera_pos(), camera->get_camera_front(), 1000.0f };
}

int ErlPhysics::check_collision(Ray* ray, std::vector<GameObject*> entities)
{
    for (int i = 0; i < entities.size(); i++)
    {
        float t1 = (entities[i]->get_aabb_min().x - ray->origin.x) / ray->direction.x;
        float t2 = (entities[i]->get_aabb_max().x - ray->origin.x) / ray->direction.x;
        float t3 = (entities[i]->get_aabb_min().y - ray->origin.y) / ray->direction.y;
        float t4 = (entities[i]->get_aabb_max().y - ray->origin.y) / ray->direction.y;
        float t5 = (entities[i]->get_aabb_min().z - ray->origin.z) / ray->direction.z;
        float t6 = (entities[i]->get_aabb_max().z - ray->origin.z) / ray->direction.z;

        float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if (tmax < 0.0f) {
            continue;
        }

        if (tmin > tmax) {
            continue;
        }

        if (tmin < 0.0f) {
            return i;
        }
        return i;
    }
        
    return -1;
}

void ErlPhysics::check_aabb_collision()
{
    for (const auto& object : physObjects)
    {
        AABB mink = calculate_minkowski_difference(playerPhysObjects.at(0)->object->get_aabb_min(), playerPhysObjects.at(0)->object->get_aabb_max(), object.second->object->get_aabb_min(), object.second->object->get_aabb_max());
        
        if(mink.min.x <= 0 && mink.max.x >= 0 &&
            mink.min.y <= 0 && mink.max.y >= 0 &&
            mink.min.z <= 0 && mink.max.z >= 0)
        {
            object.second->hit = true;
        }
        else {
            object.second->hit = false;
        }
    }
}

std::set<uint16_t> ErlPhysics::get_collided_objects()
{
    std::set<uint16_t> collidedIds;
    for (const auto& object : physObjects)
    {
        if (object.second->hit)
        {
            collidedIds.insert(object.second->object->instanceId);
        }
    }
    if (collidedIds.size() != 0) collidedIds.insert(playerPhysObjects.at(0)->object->instanceId);
    return collidedIds;
}

float ErlPhysics::check_floor_collision(glm::vec3 position, std::vector<GameObject*> entities)
{
    float objectY = position.y;
    Ray* ray = new Ray{ position, glm::vec3(0.0f, -1.0f, 0.0f), 100.0f };

    int index = check_collision(ray, entities);
    
    if (index != -1)
    {
        return entities[index]->get_aabb_max().y;
    } else {
        return objectY;
    }
}

ErlPhysics::AABB ErlPhysics::calculate_minkowski_difference(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB, glm::vec3 maxB)
{
    return AABB{ minA - maxB, maxA - minB };
}

void ErlPhysics::add_ray(ErlPhysics::Ray* ray)
{
    rays.push_back(ray);
}

std::vector<ErlPhysics::Ray*> ErlPhysics::get_rays()
{
    return rays;
}

void ErlPhysics::remove_ray_object(int index)
{
    rays.erase(rays.begin() + index);
}