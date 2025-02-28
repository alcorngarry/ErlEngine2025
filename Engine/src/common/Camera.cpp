#include"Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 50.0f, 150.0f);
	setCameraToLookAtOrigin();
}

glm::vec3 Camera::get_camera_pos()
{
	return cameraPos;
}

void Camera::set_camera_pos(glm::vec3 pos)
{
	cameraPos = pos;
}

glm::vec3 Camera::get_camera_front()
{
	return cameraFront;
}

void Camera::set_camera_front(glm::vec3 front)
{
	cameraFront = front;
}

glm::vec3 Camera::get_camera_up()
{
	return cameraUp;
}

void Camera::set_camera_up(glm::vec3 up)
{
	cameraUp = up;
}

void Camera::setCameraToLookAtOrigin() {
	cameraFront = glm::normalize(glm::vec3(0.0f) - cameraPos); // Pointing to the origin
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Set a standard up vector
}

glm::mat4 Camera::get_view_matrix()
{
	return glm::lookAt(get_camera_pos(), get_camera_pos() + get_camera_front(), get_camera_up());
	
}

glm::mat4 Camera::get_projection_matrix()
{
	//hard coded aspect ratio, figure out to transform this
	return glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 10000.0f);
}