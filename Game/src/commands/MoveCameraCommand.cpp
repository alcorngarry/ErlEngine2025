#include"MoveCameraCommand.h"

MoveCameraCommand::MoveCameraCommand(Camera* camera, float deltaTime, uint8_t direction) : camera(camera), deltaTime(deltaTime), direction(direction){}

void MoveCameraCommand::execute() 
{
	float cameraSpeed = static_cast<float>(0.005f * deltaTime);
	switch (direction)
	{
		case 0: {
			//forward
			camera->set_camera_pos(camera->get_camera_pos() + cameraSpeed * camera->get_camera_front());
			break;
		}
		case 1: {
			//backward
			camera->set_camera_pos(camera->get_camera_pos() - cameraSpeed * camera->get_camera_front());
			break;
		}
		case 2: {
			//left 
			camera->set_camera_pos(camera->get_camera_pos() - glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up())) * cameraSpeed);
			break;
		}
		case 3: {
			//right
			camera->set_camera_pos(camera->get_camera_pos() + glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up())) * cameraSpeed);
			break;
		}
		case 4: {
			float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
			float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();

			// Sensitivity adjustment for the mouse movement
			deltaX *= .001;
			deltaY *= .001;

			// Move the camera horizontally (left-right) based on mouse X movement
			camera->set_camera_pos(camera->get_camera_pos() - glm::normalize(glm::cross(camera->get_camera_front(), camera->get_camera_up())) * deltaX);

			// Move the camera vertically (up-down) based on mouse Y movement
			//camera->set_camera_up(deltaY * camera->get_camera_up());
			//camera->set_camera_pos(camera->get_camera_pos() );
			camera->set_camera_pos(camera->get_camera_pos() + deltaY * camera->get_camera_up()); // Use camera position to adjust up/down
			break;
		}
		case 5: {
			//scroll
			camera->set_camera_pos(camera->get_camera_pos() + (float)InputManager::get_scroll_value() * camera->get_camera_front());
			break;
		}
	}

	SelectEntityCommand a = SelectEntityCommand(InputManager::get_xpos(), InputManager::get_ypos(), false);
	a.execute();
}