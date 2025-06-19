#include"MoveCameraCommand.h"

MoveCameraCommand::MoveCameraCommand(Camera* camera, CameraMovement movement) : camera(camera), movement(movement)
{
	isContinuous = false;
}

void MoveCameraCommand::execute(float deltaTime)
{
	float cameraSpeed = static_cast<float>(200.0f * deltaTime);
	switch (movement)
	{
		case MOUSE_DRAG: {
			float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
			float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();

			deltaX *= 0.1f * cameraSpeed;
			deltaY *= 0.1f * cameraSpeed;

			glm::mat4 transform = camera->get_view_matrix();
			glm::vec3 right = glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up()));
			
			transform = glm::translate(transform, right * deltaX);
			transform = glm::translate(transform, glm::vec3(0.0f, -deltaY, 0.0f));

			camera->update_view_matrix(transform);
			break;
		}
		case SCROLL: {
			camera->set_camera_pos(camera->get_camera_pos() + (float)InputManager::get_scroll_value() * 30.0f * camera->get_camera_front());
			break;
		}
		case ORBIT: {
			float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
			float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();
			deltaX *= 0.1f * cameraSpeed;
			deltaY *= 0.1f * cameraSpeed;

			static float azimuth = 0.0f;
			static float elevation = 0.0f;

			azimuth += glm::radians(deltaX);
			elevation += glm::radians(deltaY);

			elevation = glm::clamp(elevation, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

			//cast ray
			ErlPhysics::Ray* orbitRay = ErlPhysics::cast_ray_from_mouse(camera, InputManager::get_ypos(), InputManager::get_last_xpos());

			glm::vec3 target = orbitRay->direction;
			float radius = glm::distance(camera->get_camera_pos(), target);
			float x = radius * cos(elevation) * sin(azimuth);
			float y = radius * sin(elevation);
			float z = radius * cos(elevation) * cos(azimuth);

			glm::vec3 cameraPos = glm::vec3(x, y, z);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::lookAt(cameraPos, target, up);

			camera->update_view_matrix(view);
			break;
		}
		case LOOK_AROUND: {
			glm::vec3 direction;
			direction.x = cos(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch()));
			direction.y = sin(glm::radians(InputManager::get_pitch()));
			direction.z = sin(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch()));
			camera->set_camera_front(glm::normalize(direction));
			break;
		}
	}
}

void MoveCameraCommand::execute(float deltaTime, float axis)
{
	float cameraSpeed = static_cast<float>(20.0f * deltaTime);
	switch (movement)
	{
		case LOOK_X: {
			InputManager::set_yaw(axis);
			glm::vec3 direction;
			direction.x = cos(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch()));
			direction.y = sin(glm::radians(InputManager::get_pitch()));
			direction.z = sin(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch()));
			camera->set_camera_front(glm::normalize(direction));
			break;
		}
		case LOOK_Y: {
			InputManager::set_pitch(-axis);
			glm::vec3 direction;
			direction.x = cos(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch()));
			direction.y = sin(glm::radians(InputManager::get_pitch()));
			direction.z = sin(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch()));
			camera->set_camera_front(glm::normalize(direction));
			break;
		}
	}
}
