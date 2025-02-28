#ifndef CAMERA_H
#define CAMERA_H
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera {
	public:
		Camera();
		glm::vec3 get_camera_up();
		void set_camera_up(glm::vec3 cameraUp);
		glm::vec3 get_camera_front();
		void set_camera_front(glm::vec3 cameraFront);
		glm::vec3 get_camera_pos();
		void set_camera_pos(glm::vec3 cameraPos);
		glm::mat4 get_view_matrix();
		glm::mat4 get_projection_matrix();
	private:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		void setCameraToLookAtOrigin();
};
#endif // !CAMERA_H
