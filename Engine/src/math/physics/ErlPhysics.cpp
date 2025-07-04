#include"ErlPhysics.h"

std::map<uint16_t, ErlPhysics::PhysicsObject*> physObjects;
std::vector<ErlPhysics::PlayerPhysicsObject*> playerPhysObjects;
std::vector<ErlPhysics::Ray*> rays;
std::vector<Camera*> physicsCameras;

void ErlPhysics::update(float deltaTime)
{
    for (PlayerPhysicsObject* playerPhysObject : playerPhysObjects)
    {
        playerPhysObject->player->update_movement(deltaTime);
        swept_aabb_collision(playerPhysObject->player, deltaTime);
    }

    
    for (const auto& npc : physObjects)
    {
        if (npc.second->object->assetId == 199)
        {
            swept_aabb_collision(npc.second->object, deltaTime);
        }
    }

    //camera collision
    /*for (Camera* camera : physicsCameras)
    {
        camera_collision(camera, deltaTime);
    }*/
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_mouse(Camera* camera, float xpos, float ypos)
{ 
	float NDC_X = ((int)xpos * (2.0f / camera->get_window_width())) - 1;
	float NDC_Y = -((int)ypos * (2.0f / camera->get_window_height())) + 1;
	float near_plane_height = glm::tan(45 / 2.0f) * 0.1f;
	float aspect_ratio = camera->get_window_width() / camera->get_window_height();

	float X_3D = NDC_X * near_plane_height * aspect_ratio;
	float Y_3D = NDC_Y * near_plane_height;

	glm::vec3 near_plane_point(X_3D, Y_3D, -0.1f);
	near_plane_point = glm::inverse(camera->get_view_matrix()) * glm::vec4(near_plane_point, 1.0f);
	glm::vec3 cam_dir_vec = near_plane_point - camera->get_camera_pos();

    return new ErlPhysics::Ray{camera->get_camera_pos(), glm::normalize(cam_dir_vec), 1000.0f};
}

void ErlPhysics::add_physics_object(GameObject* object)
{
    physObjects[object->instanceId] = new ErlPhysics::PhysicsObject{ object };
}

//add camera collision
void ErlPhysics::add_physics_camera(Camera* camera)
{
    physicsCameras.push_back(camera);
}

void ErlPhysics::remove_physics_object(uint16_t id)
{
    physObjects.erase(id);
}

void ErlPhysics::add_player_physics_object(Player* object)
{
    playerPhysObjects.push_back(new ErlPhysics::PlayerPhysicsObject{ object});
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_screen(Camera* camera, float distance)
{
    return new ErlPhysics::Ray{ camera->get_camera_pos(), camera->get_camera_front(), distance };
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_player(Player* player, float distance)
{
    return new ErlPhysics::Ray{ player->Position - glm::vec3(0.0f, (player->get_aabb_min().y - player->get_aabb_max().y) / 2.0f, 0.0f), player->wishDir, distance };
}

ErlPhysics::RayCollisionResult* ErlPhysics::check_collision(Ray* ray)
{
    int index = -1;
    float closestTmin = std::numeric_limits<float>::max();
    glm::vec3 closestPoint;

    for (const auto& physObject : physObjects)
    {
        const auto& aabbMin = physObject.second->object->get_aabb_min();
        const auto& aabbMax = physObject.second->object->get_aabb_max();

        float t1 = (aabbMin.x - ray->origin.x) / ray->direction.x;
        float t2 = (aabbMax.x - ray->origin.x) / ray->direction.x;
        float t3 = (aabbMin.y - ray->origin.y) / ray->direction.y;
        float t4 = (aabbMax.y - ray->origin.y) / ray->direction.y;
        float t5 = (aabbMin.z - ray->origin.z) / ray->direction.z;
        float t6 = (aabbMax.z - ray->origin.z) / ray->direction.z;

        float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if (tmax < 0.0f || tmin > tmax || tmin > ray->length)
            continue;

        if (tmin < closestTmin)
        {
            closestTmin = tmin;
            closestPoint = ray->origin + tmin * ray->direction;
            index = physObject.first;
        }
    }

    if (index != -1)
    {
        return new RayCollisionResult{ index,  closestPoint };
    }
    else {
        return nullptr;
    }
}

// ignoring Y values
void ErlPhysics::swept_aabb_collision(GameObject* player, float deltaTime)
{
    std::vector<SweptCollisionResult> results;

    glm::vec3 playerAABBMin = player->get_aabb_min();
    glm::vec3 playerAABBMax = player->get_aabb_max();

    for (const auto& object : physObjects)
    {
        float xInvEntry, yInvEntry, zInvEntry;
        float xInvExit, yInvExit, zInvExit;

        glm::vec3 objectAABBMin = object.second->object->get_aabb_min();
        glm::vec3 objectAABBMax = object.second->object->get_aabb_max();

        if (player->Velocity.x > 0.0f)
        {
            xInvEntry = objectAABBMin.x - playerAABBMax.x;
            xInvExit = objectAABBMax.x - playerAABBMin.x;
        }
        else {
            xInvEntry = objectAABBMax.x - playerAABBMin.x;
            xInvExit = objectAABBMin.x - playerAABBMax.x;
        }

        if (player->Velocity.y > 0.0f)
        {
            yInvEntry = objectAABBMin.y - playerAABBMax.y;
            yInvExit = objectAABBMax.y - playerAABBMin.y;
        }
        else {
            yInvEntry = objectAABBMax.y - playerAABBMin.y;
            yInvExit = objectAABBMin.y - playerAABBMax.y;
        }

        if (player->Velocity.z > 0.0f)
        {
            zInvEntry = objectAABBMin.z - playerAABBMax.z;
            zInvExit = objectAABBMax.z - playerAABBMin.z;
        }
        else {
            zInvEntry = objectAABBMax.z - playerAABBMin.z;
            zInvExit = objectAABBMin.z - playerAABBMax.z;
        }

        float xEntry, yEntry, zEntry;
        float xExit, yExit, zExit;

        if (player->Velocity.x == 0.0f)
        {
            //should probably be double
            if (playerAABBMax.x < objectAABBMin.x ||
                playerAABBMin.x > objectAABBMax.x) {
                object.second->hit = false;
                continue;
            }

            xEntry = -std::numeric_limits<float>::infinity();
            xExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            xEntry = xInvEntry / player->Velocity.x;
            xExit = xInvExit / player->Velocity.x;
        }

        if (player->Velocity.y == 0.0f)
        {
            if (playerAABBMax.y < objectAABBMin.y ||
                playerAABBMin.y > objectAABBMax.y) {
                object.second->hit = false;
                continue;
            }

            yEntry = -std::numeric_limits<float>::infinity();
            yExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            yEntry = yInvEntry / player->Velocity.y;
            yExit = yInvExit / player->Velocity.y;
        }

        if (player->Velocity.z == 0.0f)
        {
            if (playerAABBMax.z < objectAABBMin.z ||
                playerAABBMin.z > objectAABBMax.z) {
                object.second->hit = false;
                continue;
            }

            zEntry = -std::numeric_limits<float>::infinity();
            zExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            zEntry = zInvEntry / player->Velocity.z;
            zExit = zInvExit / player->Velocity.z;
        }

        float entryTime = std::max({xEntry, yEntry, zEntry});
        float exitTime = std::min({xExit, yExit, zExit});

        glm::vec3 normal = glm::vec3(0.0f);

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
                if (yInvEntry < 0.0f)
                {
                    normal += glm::vec3(0.0f, 1.0f, 0.0f);
                }
                else
                {
                    normal += glm::vec3(0.0f, -1.0f, 0.0f);
                }
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
            
            results.push_back(SweptCollisionResult{ entryTime, normal , object.first});
        }
    }

    if (!results.empty())
    {
        std::sort(results.begin(), results.end(), [](const SweptCollisionResult& a, const SweptCollisionResult& b) {
            return a.collisionTime < b.collisionTime;
            });

        for (SweptCollisionResult collision : results)
        {
            glm::vec3 contactOffset = player->Velocity * collision.collisionTime;
            player->Position += contactOffset;

            float remainingTime = 1.0f - collision.collisionTime;
            glm::vec3 remainingVelocity = player->Velocity * remainingTime;

            float intoSurface = glm::dot(remainingVelocity, collision.normal);
            glm::vec3 slideVelocity = remainingVelocity - intoSurface * collision.normal;

            player->Velocity = slideVelocity;

            handle_collision(player, physObjects[collision.objId]->object, deltaTime);
        }
    }
}

void ErlPhysics::camera_collision(Camera* camera, float deltaTime)
{
    glm::vec3 position = camera->get_camera_pos();
    Ray* ray = new Ray { position, camera->m_target - position, 1.0f };
    RayCollisionResult* result = check_collision(ray);
  
    if (result)
    {
        float distance = glm::distance(camera->m_target, result->collisionPos);

        if (distance > 10.0f)
        {
            add_ray(ray);
            std::cout << "COLLISION POS: " << std::endl;
            ErlMath::print_vector3(result->collisionPos);
            std::cout << "CAMERA COLLISION: " << distance << std::endl;
            camera->followRadius = distance / 2;
            camera->update_follow_position();

            //call this method again
            //camera_collision(camera, false, deltaTime);
        }
    }
    else {
        camera->followRadius = 200.0f;
        camera->update_follow_position();
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

float ErlPhysics::check_floor_collision(Player* player)
{
    float objectY = player->Position.y;
    Ray* ray = new Ray{ player->Position, glm::vec3(0.0f, -1.0f, 0.0f), 1000.0f };
    RayCollisionResult* result = check_collision(ray);
    if (result)
    {
        if (player->Position.y > physObjects[result->index]->object->get_aabb_max().y) player->onGround = false;
        return physObjects[result->index]->object->get_aabb_max().y;
    } else {
        return player->floorHeight;
    }
}

//fix this REMOVE!
float ErlPhysics::check_floor_collision(NPC* npc)
{
    float objectY = npc->Position.y;
    Ray* ray = new Ray{ npc->Position, glm::vec3(0.0f, -1.0f, 0.0f), 1000.0f };
    RayCollisionResult* result = check_collision(ray);
    if (result)
    {
        if (npc->Position.y > physObjects[result->index]->object->get_aabb_max().y) npc->onGround = false;
        return physObjects[result->index]->object->get_aabb_max().y;
    }
    else {
        return npc->floorHeight;
    }
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

void ErlPhysics::handle_collision(GameObject* a, GameObject* b, float deltaTime)
{
    a->on_collision(b, deltaTime);
    b->on_collision(a, deltaTime);
}