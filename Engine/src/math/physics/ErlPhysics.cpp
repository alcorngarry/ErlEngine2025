#include"ErlPhysics.h"

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

int ErlPhysics::check_aabb_collision(glm::vec3 targetMin, glm::vec3 targetMax, std::vector<GameObject*> entities)
{
    for (GameObject* object : entities)
    {
        AABB mink = calculate_minkowski_difference(targetMin, targetMax, object->get_aabb_min(), object->get_aabb_max());
        
        if(mink.min.x <= 0 && mink.max.x >= 0 &&
            mink.min.y <= 0 && mink.max.y >= 0 &&
            mink.min.z <= 0 && mink.max.z >= 0)
        {
            return object->instanceId;
        }
    }
    return -1;
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

glm::vec3 ErlPhysics::resolve_aabb_collision(glm::vec3 position, glm::vec3 targetMin, glm::vec3 targetMax, GameObject* object)
{
    glm::vec3 objectMin = object->get_aabb_min();
    glm::vec3 objectMax = object->get_aabb_max();
    glm::vec3 overlap(0.0f);

    if (targetMax.x > objectMin.x && targetMin.x < objectMax.x) {
        if (targetMax.x > objectMax.x) {
            overlap.x = objectMax.x - targetMin.x;
        }
        else if (targetMin.x < objectMin.x) {
            overlap.x = targetMax.x - objectMin.x;
        }
    }

    if (targetMax.y > objectMin.y && targetMin.y < objectMax.y) {
        if (targetMax.y > objectMax.y) {
            overlap.y = objectMax.y - targetMin.y;
        }
        else if (targetMin.y < objectMin.y) {
            overlap.y = targetMax.y - objectMin.y;
        }
    }

    if (targetMax.z > objectMin.z && targetMin.z < objectMax.z) {
        if (targetMax.z > objectMax.z) {
            overlap.z = objectMax.z - targetMin.z;
        }
        else if (targetMin.z < objectMin.z) {
            overlap.z = targetMax.z - objectMin.z;
        }
    }

    glm::vec3 moveAmount(0.0f);

    if (std::abs(overlap.x) < std::abs(overlap.y) && std::abs(overlap.x) < std::abs(overlap.z)) {
        moveAmount.x = overlap.x;
    }

    if (std::abs(overlap.y) < std::abs(overlap.x) && std::abs(overlap.y) < std::abs(overlap.z)) {
        moveAmount.y = overlap.y;
    }

    if (std::abs(overlap.z) < std::abs(overlap.x) && std::abs(overlap.z) < std::abs(overlap.y)) {
        moveAmount.z = overlap.z;
    }

    glm::vec3 slideDirection = glm::normalize(moveAmount);

    if (moveAmount.x != 0.0f) {
        moveAmount.x += slideDirection.x;
    }

    if (moveAmount.y != 0.0f) {
        moveAmount.y += slideDirection.y;
    }

    if (moveAmount.z != 0.0f) {
        moveAmount.z += slideDirection.z;
    }

    std::cout << moveAmount.x << ", " << moveAmount.y << ", " << moveAmount.z << std::endl;
    return moveAmount;
}

void test(glm::vec3 pos, glm::vec3 min, glm::vec3 max, GameObject* object)
{
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;
    //VECTOR planeOrigin = intersectionPoint;
    //VECTOR planeNormal = newPosition - intersectionPoint;
    //planeNormal.normalize();
    //PLANE slidingPlane(planeOrigin, planeNormal);
}

ErlPhysics::AABB ErlPhysics::calculate_minkowski_difference(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB, glm::vec3 maxB)
{
    return AABB{ minA - maxB, maxA - minB };
}

ErlPhysics::CollisionResult ErlPhysics::sweep_aabb(glm::vec3 minA, glm::vec3 maxA, glm::vec3 velocity, std::vector<GameObject*> entities)
{
    CollisionResult result{};
    result.hit = false;
    result.timeOfImpact = 1.0f;

    glm::vec3 extentsA = (maxA - minA) * 0.5f;
    glm::vec3 centerA = minA + extentsA;

    for (GameObject* obj : entities)
    {
        glm::vec3 minB = obj->get_aabb_min();
        glm::vec3 maxB = obj->get_aabb_max();

        glm::vec3 minkMin = minB - extentsA;
        glm::vec3 minkMax = maxB + extentsA;

        glm::vec3 invDir = 1.0f / velocity;

        glm::vec3 t1 = (minkMin - centerA) * invDir;
        glm::vec3 t2 = (minkMax - centerA) * invDir;

        glm::vec3 tMin = glm::min(t1, t2);
        glm::vec3 tMax = glm::max(t1, t2);

        float entryTime = std::max(std::max(tMin.x, tMin.y), tMin.z);
        float exitTime = std::min(std::min(tMax.x, tMax.y), tMax.z);

        if (exitTime >= 0.0f && entryTime < result.timeOfImpact && entryTime <= exitTime && entryTime >= 0.0f)
        {
            result.hit = true;
            result.timeOfImpact = entryTime;
            result.hitObject = obj;

            if (entryTime == tMin.x) result.normal = glm::vec3((velocity.x < 0) ? 1 : -1, 0, 0);
            else if (entryTime == tMin.y) result.normal = glm::vec3(0, (velocity.y < 0) ? 1 : -1, 0);
            else result.normal = glm::vec3(0, 0, (velocity.z < 0) ? 1 : -1);
        }
    }

    if (result.hit) {
        result.contactPoint = centerA + velocity * result.timeOfImpact;
    }

    return result;
}