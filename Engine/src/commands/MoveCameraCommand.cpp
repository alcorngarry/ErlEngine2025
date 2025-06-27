#include"MoveCameraCommand.h"

MoveCameraCommand::MoveCameraCommand(Camera* camera, CameraMovement movement) : camera(camera), movement(movement)
{
	isContinuous = false;
}

void MoveCameraCommand::execute(float deltaTime)
{
	float cameraSpeed = static_cast<float>(500.0f * deltaTime);
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
			camera->set_camera_pos(camera->get_camera_pos() + (float)InputManager::get_scroll_value() * 100.0f * camera->get_camera_front());
			break;
		}
		case ORBIT: {
			float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
			float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();
			float sensitivity = 0.005f;

			camera->azimuth += deltaX * sensitivity;
			camera->elevation += deltaY * sensitivity;

			glm::vec3 target = glm::vec3(0.0f);
			camera->followRadius = glm::distance(camera->get_camera_pos(), target);
			camera->follow_position(target);
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
	float sensitivity = 1.5f;

	switch (movement)
	{
		case LOOK_Y: {
			camera->elevation += axis * deltaTime * sensitivity;
			camera->elevation = glm::clamp(camera->elevation, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);
			camera->update_follow_position();
			break;
		}
		case LOOK_X: {
			camera->azimuth += axis * deltaTime * sensitivity;
			camera->update_follow_position();
			break;
		}
	}
}

