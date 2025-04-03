#include"ErlPhysics.h"

void ErlPhysics::cast_ray_from_mouse(Camera* camera, float xpos, float ypos)
{
	float NDC_X = ((int)xpos * (2.0f / Renderer::get_window_width())) - 1;
	float NDC_Y = -((int)ypos * (2.0f / Renderer::get_window_height())) + 1;
	float near_plane_height = glm::tan(45 / 2.0f) * 0.1f;
	float aspect_ratio = Renderer::get_window_width() / Renderer::get_window_height();

	float X_3D = NDC_X * near_plane_height * aspect_ratio;
	float Y_3D = NDC_Y * near_plane_height;

	glm::vec3 near_plane_point(X_3D, Y_3D, -0.1f);
	near_plane_point = glm::inverse(camera->get_view_matrix()) * glm::vec4(near_plane_point, 1.0f);
	glm::vec3 cam_dir_vec = near_plane_point - camera->get_camera_pos();

	Renderer::add_ray(camera->get_camera_pos(), glm::normalize(cam_dir_vec), 1000.0f);
}

void ErlPhysics::cast_ray_from_screen(Camera* camera)
{
	Renderer::add_ray(camera->get_camera_pos(), camera->get_camera_front(), 1000.0f);
}

GameObject* ErlPhysics::check_collision(Renderer::Ray* ray, std::vector<GameObject*> entities)
{
    for (GameObject* entity : entities)
    {
        float t1 = (entity->get_aabb_min().x - ray->origin.x) / ray->direction.x * ray->length;
        float t2 = (entity->get_aabb_max().x - ray->origin.x) / ray->direction.x * ray->length;
        float t3 = (entity->get_aabb_min().y - ray->origin.y) / ray->direction.y * ray->length;
        float t4 = (entity->get_aabb_max().y - ray->origin.y) / ray->direction.y * ray->length;
        float t5 = (entity->get_aabb_min().z - ray->origin.z) / ray->direction.z * ray->length;
        float t6 = (entity->get_aabb_max().z - ray->origin.z) / ray->direction.z * ray->length;

        float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
        if (tmax < 0.0f) {
            break;
        }

        // if tmin > tmax, ray doesn't intersect AABB
        if (tmin > tmax) {
            break;
        }

        if (tmin < 0.0f) {
            return entity;
        }
        return entity;
    }
}