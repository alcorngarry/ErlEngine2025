#include"MoveCameraCommand.h"

MoveCameraCommand::MoveCameraCommand(Camera* camera, float deltaTime, Movement movement) : camera(camera), deltaTime(deltaTime), movement(movement)
{
	/*switch (movement)
	{
		case FORWARD: isContinuous = true;
		case BACK: isContinuous = true;
		case LEFT: isContinuous = true;
		case RIGHT: isContinuous = true;
		default:;
	}*/

	isContinuous = true;
		
}

void MoveCameraCommand::execute() 
{
	float cameraSpeed = static_cast<float>(100.0f * deltaTime);
	switch (movement)
	{
		case FORWARD: {
			camera->set_camera_pos(camera->get_camera_pos() + cameraSpeed * glm::vec3(camera->get_camera_front().x, 0.0f, camera->get_camera_front().z));
			break;
		}
		case BACK: {
			camera->set_camera_pos(camera->get_camera_pos() - cameraSpeed * glm::vec3(camera->get_camera_front().x, 0.0f, camera->get_camera_front().z));
			break;
		}
		case LEFT: {
			camera->set_camera_pos(camera->get_camera_pos() - glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up())) * cameraSpeed);
			break;
		}
		case RIGHT: {
			camera->set_camera_pos(camera->get_camera_pos() + glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up())) * cameraSpeed);
			break;
		}
		case MOUSE_DRAG: {
			float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
			float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();

			deltaX *= 0.1f;
			deltaY *= 0.1f;

			glm::mat4 transform = camera->get_view_matrix();
			glm::vec3 right = glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up()));
			
			transform = glm::translate(transform, right * deltaX);
			transform = glm::translate(transform, glm::vec3(0.0f, -deltaY, 0.0f));

			camera->update_view_matrix(transform);
			break;
		}
		case SCROLL: {
			camera->set_camera_pos(camera->get_camera_pos() + (float)InputManager::get_scroll_value() * 10 * camera->get_camera_front());
			break;
		}
		case ORBIT: {
			float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
			float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();
			deltaX *= .1;
			deltaY *= .1;

			if (deltaX != 0.0f)
			{
				float camX = sin(deltaX);
				float camZ = cos(deltaX);

				glm::mat4 transform = camera->get_view_matrix();

				transform *= glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
				camera->update_view_matrix(transform);
			}
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