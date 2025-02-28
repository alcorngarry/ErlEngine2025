#include"MoveCameraCommand.h"

MoveCameraCommand::MoveCameraCommand(Camera* camera, float deltaTime, uint8_t direction) : camera(camera), deltaTime(deltaTime), direction(direction){}

void MoveCameraCommand::execute() 
{
	float cameraSpeed = static_cast<float>(0.5f * deltaTime);
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
		/*case 4: {
			if (InputManager::is_mouse_button_pressed()) {
				float deltaX = InputManager::get_xpos() - InputManager::get_last_xpos();
				float deltaY = InputManager::get_ypos() - InputManager::get_last_ypos();

				std::cout << "LINE HIT" << std::endl;
				camera->set_camera_pos(camera->get_camera_pos() + deltaX * glm::normalize(glm::cross(camera->get_camera_front(), deltaY + camera->get_camera_up())) * cameraSpeed);
			}
			break;
		}*/
	}
}