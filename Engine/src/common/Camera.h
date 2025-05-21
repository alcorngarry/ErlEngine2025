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
		Camera(float windowWidth, float windowHeight);
		glm::vec3 get_camera_pos() const;
		void set_camera_pos(glm::vec3 cameraPos);
		glm::mat4 get_view_matrix() const;
		void update_view_matrix(glm::mat4 view);
		glm::mat4 get_projection_matrix() const;
		void update_view_matrix();

		void look_at(glm::vec3 lookAtPosition);
		
		float m_windowHeight;
		float m_windowWidth;
	private:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		glm::mat4 m_view;
		glm::mat4 m_projection;
		
		void setCameraToLookAtOrigin();
};
#endif // !CAMERA_H
