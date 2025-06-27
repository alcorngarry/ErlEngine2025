#include"ErlPhysics.h"

std::map<uint16_t, ErlPhysics::PhysicsObject*> physObjects;
std::vector<ErlPhysics::PlayerPhysicsObject*> playerPhysObjects;
std::vector<ErlPhysics::Ray*> rays;

void ErlPhysics::update(float deltaTime)
{
    for (PlayerPhysicsObject* playerPhysObject : playerPhysObjects)
    {
        playerPhysObject->player->update_movement(deltaTime);
        swept_aabb_collision(playerPhysObject->player, deltaTime);
    }
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
    physObjects[object->instanceId] = new ErlPhysics::PhysicsObject{ object };
}


void ErlPhysics::remove_physics_object(uint16_t id)
{
    physObjects.erase(id);
}

void ErlPhysics::add_player_physics_object(Player* object)
{
    playerPhysObjects.push_back(new ErlPhysics::PlayerPhysicsObject{ object});
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_screen(Camera* camera)
{
    return new ErlPhysics::Ray{ camera->get_camera_pos(), camera->get_camera_front(), 1000.0f };
}

ErlPhysics::Ray* ErlPhysics::cast_ray_from_player(Player* player)
{
    return new ErlPhysics::Ray{ player->Position, player->wishDir, 1000.0f };
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

        if (physObject.second->object->get_aabb_max().y > maxHeight)
        {
            maxHeight = physObject.second->object->get_aabb_max().y;
            index = physObject.first;
        }
    }
    return index;
}

// ignoring Y values
void ErlPhysics::swept_aabb_collision(Player* player, float deltaTime)
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
    Ray* ray = new Ray{ player->Position, glm::vec3(0.0f, -1.0f, 0.0f), 100.0f };

    int index = check_collision(ray);
    if (index != -1)
    {
        if (player->Position.y > physObjects[index]->object->get_aabb_max().y) player->onGround = false;
        return physObjects[index]->object->get_aabb_max().y;
    } else {
        return player->floorHeight;
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