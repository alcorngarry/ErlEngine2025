#include"MoveCameraCommand.h"

MoveCameraCommand::MoveCameraCommand(Camera* camera, float deltaTime, Movement movement) : camera(camera), deltaTime(deltaTime), movement(movement){}

void MoveCameraCommand::execute() 
{
	float cameraSpeed = static_cast<float>(0.005f * deltaTime);
	switch (movement)
	{
		case FORWARD: {
			//forward
			camera->set_camera_pos(camera->get_camera_pos() + cameraSpeed * camera->get_camera_front());
			break;
		}
		case BACK: {
			//backward
			camera->set_camera_pos(camera->get_camera_pos() - cameraSpeed * camera->get_camera_front());
			break;
		}
		case LEFT: {
			//left 
			camera->set_camera_pos(camera->get_camera_pos() - glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up())) * cameraSpeed);
			break;
		}
		case RIGHT: {
			//right
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
	}
}