#include"ErlPhysics.h"

std::map<uint16_t, ErlPhysics::PhysicsObject*> physObjects;
std::vector<ErlPhysics::PlayerPhysicsObject*> playerPhysObjects;
std::vector<ErlPhysics::Ray*> rays;

void ErlPhysics::update(float deltaTime)
{
   /* if(rays.size() > 1) rays.pop_back();
    rays.push_back(new Ray{ playerPhysObjects.at(0)->player->Position, glm::normalize(playerPhysObjects.at(0)->player->Velocity), glm::length(playerPhysObjects.at(0)->player->Position + playerPhysObjects.at(0)->player->Velocity) });*/
    //check_aabb_collision(deltaTime);

    playerPhysObjects.at(0)->player->update_movement(deltaTime);
    swept_aabb_collision(deltaTime);
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
    physObjects[object->instanceId] = new ErlPhysics::PhysicsObject{ new AABB { 
        playerPhysObjects.at(0)->player->get_aabb_max() + object->get_aabb_max(), 
        playerPhysObjects.at(0)->player->get_aabb_min() + object->get_aabb_min()},
        object 
    };
}

void ErlPhysics::add_player_physics_object(Player* object)
{
    playerPhysObjects.push_back(new ErlPhysics::PlayerPhysicsObject{ nullptr, object});
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_screen(Camera* camera)
{
    return new ErlPhysics::Ray{ camera->get_camera_pos(), camera->get_camera_front(), 1000.0f };
}

int ErlPhysics::check_collision(Ray* ray)
{
    float maxHeight = -1000.0f; //fix this later
    int index = -1;
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
        //std::max(maxHeight, physObject.second->object->get_aabb_max().y);
        if (physObject.second->object->get_aabb_max().y > maxHeight)
        {
            maxHeight = physObject.second->object->get_aabb_max().y;
            index = physObject.first;
        }
    }
    return index;
}

void ErlPhysics::check_aabb_collision(float deltaTime)
{
    for (const auto& object : physObjects)
    {
        AABB mink = calculate_minkowski_difference(playerPhysObjects.at(0)->player->get_aabb_min(), playerPhysObjects.at(0)->player->get_aabb_max(), object.second->object->get_aabb_min(), object.second->object->get_aabb_max());
        
        if(mink.min.x <= 0 && mink.max.x >= 0 &&
            mink.min.y <= 0 && mink.max.y >= 0 &&
            mink.min.z <= 0 && mink.max.z >= 0)
        {
            object.second->hit = true;
            //resolve_collision(mink, deltaTime);
        }
        else {
            object.second->hit = false;
        }
    }
}

// ignoring Y values
void ErlPhysics::swept_aabb_collision(float deltaTime)
{
    std::vector<SweptCollisionResult> results;
    for (const auto& object : physObjects)
    {
        float xInvEntry, yInvEntry, zInvEntry;
        float xInvExit, yInvExit, zInvExit;

        if (playerPhysObjects.at(0)->player->Velocity.x > 0.0f)
        {
            xInvEntry = object.second->object->get_aabb_min().x - playerPhysObjects.at(0)->player->get_aabb_max().x;
            xInvExit = object.second->object->get_aabb_max().x - playerPhysObjects.at(0)->player->get_aabb_min().x;
        }
        else {
            xInvEntry = object.second->object->get_aabb_max().x - playerPhysObjects.at(0)->player->get_aabb_min().x;
            xInvExit = object.second->object->get_aabb_min().x - playerPhysObjects.at(0)->player->get_aabb_max().x;
        }

        if (playerPhysObjects.at(0)->player->Velocity.y > 0.0f)
        {
            yInvEntry = object.second->object->get_aabb_min().y - playerPhysObjects.at(0)->player->get_aabb_max().y;
            yInvExit = object.second->object->get_aabb_max().y - playerPhysObjects.at(0)->player->get_aabb_min().y;
        }
        else {
            yInvEntry = object.second->object->get_aabb_max().y - playerPhysObjects.at(0)->player->get_aabb_min().y;
            yInvExit = object.second->object->get_aabb_min().y - playerPhysObjects.at(0)->player->get_aabb_max().y;
        }

        if (playerPhysObjects.at(0)->player->Velocity.z > 0.0f)
        {
            zInvEntry = object.second->object->get_aabb_min().z - playerPhysObjects.at(0)->player->get_aabb_max().z;
            zInvExit = object.second->object->get_aabb_max().z - playerPhysObjects.at(0)->player->get_aabb_min().z;
        }
        else {
            zInvEntry = object.second->object->get_aabb_max().z - playerPhysObjects.at(0)->player->get_aabb_min().z;
            zInvExit = object.second->object->get_aabb_min().z - playerPhysObjects.at(0)->player->get_aabb_max().z;
        }

        float xEntry, yEntry, zEntry;
        float xExit, yExit, zExit;

        if (playerPhysObjects.at(0)->player->Velocity.x == 0.0f)
        {
            //should probably be double
            if (playerPhysObjects.at(0)->player->get_aabb_max().x < object.second->object->get_aabb_min().x ||
                playerPhysObjects.at(0)->player->get_aabb_min().x > object.second->object->get_aabb_max().x) {
                object.second->hit = false;
                continue;
            }

            xEntry = -std::numeric_limits<float>::infinity();
            xExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            xEntry = xInvEntry / playerPhysObjects.at(0)->player->Velocity.x;
            xExit = xInvExit / playerPhysObjects.at(0)->player->Velocity.x;
        }

        if (playerPhysObjects.at(0)->player->Velocity.y == 0.0f)
        {
            if (playerPhysObjects.at(0)->player->get_aabb_max().y < object.second->object->get_aabb_min().y || 
                playerPhysObjects.at(0)->player->get_aabb_min().y > object.second->object->get_aabb_max().y) {
                object.second->hit = false;
                continue;
            }

            yEntry = -std::numeric_limits<float>::infinity();
            yExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            yEntry = yInvEntry / playerPhysObjects.at(0)->player->Velocity.y;
            yExit = yInvExit / playerPhysObjects.at(0)->player->Velocity.y;
        }

        if (playerPhysObjects.at(0)->player->Velocity.z == 0.0f)
        {
            if (playerPhysObjects.at(0)->player->get_aabb_max().z < object.second->object->get_aabb_min().z ||
                playerPhysObjects.at(0)->player->get_aabb_min().z > object.second->object->get_aabb_max().z) {
                object.second->hit = false;
                continue;
            }

            zEntry = -std::numeric_limits<float>::infinity();
            zExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            zEntry = zInvEntry / playerPhysObjects.at(0)->player->Velocity.z;
            zExit = zInvExit / playerPhysObjects.at(0)->player->Velocity.z;
        }

        float entryTime = std::max({xEntry, yEntry, zEntry});
        float exitTime = std::min({xExit, yExit, zExit});

        //float entryTime = std::max({ xEntry, zEntry });
        //float exitTime = std::min({ xExit, zExit });

        glm::vec3 normal = glm::vec3(0.0f);

        //no collision
        if (entryTime > exitTime || entryTime < 0.0f || entryTime > deltaTime)
        {
            object.second->hit = false;
        }
        else {
            object.second->hit = true;
            if (xEntry > zEntry)
            {
                if (xInvEntry < 0.0f)
                {
                    normal += glm::vec3(1.0f, 0.0f, 0.0f);
                }
                else
                {
                    normal += glm::vec3(-1.0f, 0.0f, 0.0f);
                }
            }
            else if (yEntry > xEntry && yEntry > zEntry)
            {
                /*if (yInvEntry < 0.0f)
                {
                    normal += glm::vec3(0.0f, 1.0f, 0.0f);
                }
                else
                {
                    normal += glm::vec3(0.0f, -1.0f, 0.0f);
                }*/
            }
            else if (zEntry > xEntry) {
                if (zInvEntry < 0.0f)
                {
                    normal += glm::vec3(0.0f, 0.0f, 1.0f);
                }
                else
                {
                    normal += glm::vec3(0.0f, 0.0f, -1.0f);
                }
            }
            results.push_back(SweptCollisionResult{ entryTime, normal });
        }
    }

    
    if (!results.empty())
    {
        std::sort(results.begin(), results.end(), [](const SweptCollisionResult& a, const SweptCollisionResult& b) {
            return a.collisionTime < b.collisionTime;
            });

        for (SweptCollisionResult collision : results)
        {
            glm::vec3 contactOffset = playerPhysObjects.at(0)->player->Velocity * collision.collisionTime;
            playerPhysObjects.at(0)->player->Position += contactOffset;

            float remainingTime = 1.0f - collision.collisionTime;
            glm::vec3 remainingVelocity = playerPhysObjects.at(0)->player->Velocity * remainingTime;

            float intoSurface = glm::dot(remainingVelocity, collision.normal);
            glm::vec3 slideVelocity = remainingVelocity - intoSurface * collision.normal;

            playerPhysObjects.at(0)->player->Velocity = slideVelocity;
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
    if (collidedIds.size() != 0) collidedIds.insert(playerPhysObjects.at(0)->player->instanceId);
    return collidedIds;
}

float ErlPhysics::check_floor_collision()
{
    float objectY = playerPhysObjects.at(0)->player->Position.y;
    Ray* ray = new Ray{ playerPhysObjects.at(0)->player->Position, glm::vec3(0.0f, -1.0f, 0.0f), 100.0f };
    //rays.push_back(ray);
    int index = check_collision(ray);
    
    if (index != -1)
    {
        if (playerPhysObjects.at(0)->player->Position.y > physObjects[index]->object->get_aabb_max().y + playerPhysObjects.at(0)->player->playerHeight) playerPhysObjects.at(0)->player->onGround = false;
        return physObjects[index]->object->get_aabb_max().y;
    } else {
        return playerPhysObjects.at(0)->player->floorHeight;
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