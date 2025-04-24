#include"ErlPhysics.h"

std::map<uint16_t, ErlPhysics::PhysicsObject*> physObjects;
std::vector<ErlPhysics::PhysicsObject*> playerPhysObjects;
std::vector<ErlPhysics::Ray*> rays;

void ErlPhysics::update(float deltaTime)
{
    check_aabb_collision(deltaTime);
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

int ErlPhysics::check_collision(Ray* ray)
{
    for (const auto& physObject : physObjects)
    {
        float t1 = (physObject.second->object->get_aabb_min().x - ray->origin.x) / ray->direction.x;
        float t2 = (physObject.second->object->get_aabb_max().x - ray->origin.x) / ray->direction.x;
        float t3 = (physObject.second->object->get_aabb_min().y - ray->origin.y) / ray->direction.y;
        float t4 = (physObject.second->object->get_aabb_max().y - ray->origin.y) / ray->direction.y;
        float t5 = (physObject.second->object->get_aabb_min().z - ray->origin.z) / ray->direction.z;
        float t6 = (physObject.second->object->get_aabb_max().z - ray->origin.z) / ray->direction.z;

        float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if (tmax < 0.0f) {
            continue;
        }

        if (tmin > tmax) {
            continue;
        }

        return physObject.first;
    }
        
    return -1;
}

void ErlPhysics::check_aabb_collision(float deltaTime)
{
    for (const auto& object : physObjects)
    {
        AABB mink = calculate_minkowski_difference(playerPhysObjects.at(0)->object->get_aabb_min(), playerPhysObjects.at(0)->object->get_aabb_max(), object.second->object->get_aabb_min(), object.second->object->get_aabb_max());
        
        if(mink.min.x <= 0 && mink.max.x >= 0 &&
            mink.min.y <= 0 && mink.max.y >= 0 &&
            mink.min.z <= 0 && mink.max.z >= 0)
        {
            object.second->hit = true;
            resolve_collision(mink, deltaTime);
        }
        else {
            object.second->hit = false;
        }
    }
}

void ErlPhysics::resolve_collision(AABB mink, float deltaTime)
{
    glm::vec3 playerPosition = playerPhysObjects.at(0)->object->Position;
    glm::vec3 playerVelocity = playerPhysObjects.at(0)->object->Velocity;

    glm::vec3 separation(0.0f);  // Separation vector to resolve overlap
    glm::vec3 normal;            // Collision normal

    float minDist = std::numeric_limits<float>::max();
    glm::vec3 boundsPoint;

    if (std::abs(playerPosition.x - mink.min.x) < minDist) {
        minDist = std::abs(playerPosition.x - mink.min.x);
        boundsPoint = glm::vec3(mink.min.x, playerPosition.y, playerPosition.z);
        separation.x = mink.min.x - playerPosition.x;  // Resolve overlap along X-axis
        normal = glm::vec3(1.0f, 0.0f, 0.0f);  // X-axis normal
    }
    if (std::abs(playerPosition.x - mink.max.x) < minDist) {
        minDist = std::abs(playerPosition.x - mink.max.x);
        boundsPoint = glm::vec3(mink.max.x, playerPosition.y, playerPosition.z);
        separation.x = mink.max.x - playerPosition.x;  // Resolve overlap along X-axis
        normal = glm::vec3(-1.0f, 0.0f, 0.0f);  // X-axis normal
    }

    if (std::abs(playerPosition.y - mink.min.y) < minDist) {
        minDist = std::abs(playerPosition.y - mink.min.y);
        boundsPoint = glm::vec3(playerPosition.x, mink.min.y, playerPosition.z);
        separation.y = mink.min.y - playerPosition.y;  // Resolve overlap along Y-axis
        normal = glm::vec3(0.0f, 1.0f, 0.0f);  // Y-axis normal
    }
    if (std::abs(playerPosition.y - mink.max.y) < minDist) {
        minDist = std::abs(playerPosition.y - mink.max.y);
        boundsPoint = glm::vec3(playerPosition.x, mink.max.y, playerPosition.z);
        separation.y = mink.max.y - playerPosition.y;  // Resolve overlap along Y-axis
        normal = glm::vec3(0.0f, -1.0f, 0.0f);  // Y-axis normal
    }

    if (std::abs(playerPosition.z - mink.min.z) < minDist) {
        minDist = std::abs(playerPosition.z - mink.min.z);
        boundsPoint = glm::vec3(playerPosition.x, playerPosition.y, mink.min.z);
        separation.z = mink.min.z - playerPosition.z;  // Resolve overlap along Z-axis
        normal = glm::vec3(0.0f, 0.0f, 1.0f);  // Z-axis normal
    }
    if (std::abs(playerPosition.z - mink.max.z) < minDist) {
        minDist = std::abs(playerPosition.z - mink.max.z);
        boundsPoint = glm::vec3(playerPosition.x, playerPosition.y, mink.max.z);
        separation.z = mink.max.z - playerPosition.z;  // Resolve overlap along Z-axis
        normal = glm::vec3(0.0f, 0.0f, -1.0f);  // Z-axis normal
    }

    playerPhysObjects.at(0)->object->Position += separation;
    playerPhysObjects.at(0)->object->Velocity -= glm::dot(playerVelocity, normal) * normal;
    playerPhysObjects.at(0)->object->Velocity *= 0.9f;  // Friction coefficient (optional)
    playerPhysObjects.at(0)->object->Velocity *= deltaTime;
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

float ErlPhysics::check_floor_collision()
{
    float objectY = playerPhysObjects.at(0)->object->Position.y;
    Ray* ray = new Ray{ playerPhysObjects.at(0)->object->Position, glm::vec3(0.0f, -1.0f, 0.0f), 100.0f };

    int index = check_collision(ray);
    
    if (index != -1)
    {
        return physObjects[index]->object->get_aabb_max().y;
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