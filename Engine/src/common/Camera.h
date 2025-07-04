#ifndef CAMERA_H
#define CAMERA_H
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera {
	public:
		glm::vec3 get_camera_up() const;
		void set_camera_up(glm::vec3 cameraUp);
		glm::vec3 get_camera_front() const;
		void set_camera_front(glm::vec3 cameraFront);
		Camera(GLFWwindow* window);
		glm::vec3 get_camera_pos() const;
		float get_window_width() const;
		float get_window_height() const;
		void set_camera_pos(glm::vec3 cameraPos);
		glm::mat4 get_view_matrix() const;
		glm::vec3 get_target_pos();
		void update_view_matrix(glm::mat4 view);
		glm::mat4 get_projection_matrix() const;
		void update_view_matrix();
		void follow_position(glm::vec3 followPosition);
		void update_follow_position();

		void setCameraFront(const glm::mat4& viewMatrix);
		
		float followRadius, azimuth, elevation;
		glm::vec3 m_target;
	private:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		glm::mat4 m_view;
		GLFWwindow* m_window;

		void setCameraToLookAtOrigin();
};
#endif // !CAMERA_H
