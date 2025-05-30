#include "Camera.h"

Camera::Camera(float windowWidth, float windowHeight)
{
    m_windowHeight = windowHeight;
    m_windowWidth = windowWidth;
    m_projection = glm::perspective(glm::radians(45.0f), m_windowWidth / m_windowHeight, 0.1f, 10000.0f);
    setCameraToLookAtOrigin();
}

glm::vec3 Camera::get_camera_pos() const
{
    return cameraPos;
}

void Camera::set_camera_pos(glm::vec3 pos)
{
    cameraPos = pos;
    update_view_matrix();
}

glm::vec3 Camera::get_camera_front() const
{
    return cameraFront;
}

void Camera::set_camera_front(glm::vec3 front)
{
    cameraFront = glm::normalize(front);
    update_view_matrix();
}

glm::vec3 Camera::get_camera_up() const
{
    return cameraUp;
}

void Camera::set_camera_up(glm::vec3 up)
{
    cameraUp = glm::normalize(up);
    update_view_matrix();
}

void Camera::setCameraToLookAtOrigin()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    update_view_matrix();
}

glm::mat4 Camera::get_view_matrix() const
{
    return m_view;
}

void Camera::update_view_matrix(glm::mat4 view)
{
    m_view = view;

    glm::mat3 rotationMatrix = glm::mat3(view);
    cameraFront = -glm::vec3(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]);
    cameraUp = glm::vec3(rotationMatrix[0][1], rotationMatrix[1][1], rotationMatrix[2][1]);
    glm::mat4 inverseView = glm::inverse(view);
    cameraPos = glm::vec3(inverseView[3][0], inverseView[3][1], inverseView[3][2]);
}

glm::mat4 Camera::get_projection_matrix() const
{
    return m_projection;
}

void Camera::update_view_matrix()
{
    m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::follow_position(glm::vec3 followPosition)
{
    set_camera_pos(followPosition + glm::vec3(0.0f, 100.0f, 200.0f));
    m_view = glm::lookAt(get_camera_pos(), followPosition, glm::vec3(0.0f, 10.0f, 0.0f));
}