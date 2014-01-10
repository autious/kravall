#include "Camera.hpp"
#include <algorithm>

namespace Core
{
	Camera* gameCamera;

	Camera::Camera(const float& fov, const float& nearZ, const float& farZ)
	{
		m_fov = fov;
		m_near = nearZ;
		m_far = farZ;
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);

		m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_up =		glm::vec3(0.0f, 1.0f, 0.0f);

		m_viewMatrix = glm::mat4x4(1.0f);
		m_projectionMatrix = glm::mat4x4(1.0f);
	}

	Camera::~Camera()
	{

	}

	void Camera::LookAt(glm::vec3 lookAtTarget, glm::vec3 up)
	{
		m_forward = glm::normalize(m_position + lookAtTarget);
		m_up = up;
		CalculateViewMatrix();
	}

    void Camera::SetDirection( glm::vec3 forward, glm::vec3 up )
    {
        m_forward = forward;
        m_up = up;
		CalculateViewMatrix();
    }

	void Camera::CalculateViewMatrix()
	{
		m_viewMatrix = glm::lookAt<float>(m_position, m_forward, m_up);

	}

	void Camera::CalculateProjectionMatrix(const unsigned int &width, const unsigned int &height)
	{
		// TODO: Verify
		m_projectionMatrix = glm::perspective<float>(m_fov, 
			static_cast<float>(width)/static_cast<float>(height), m_near, m_far);
	}

	void Camera::CalculateProjectionOrtho(const unsigned int &width, const unsigned int &height)
	{
		// TODO: Verify
		int halfWidth = width / 2;
		int halfHeight = height / 2;
		m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight);
	}

	void Camera::UpdateView(glm::vec3 directions, glm::vec2 rotations, float dt)
	{
		glm::quat pitch = glm::quat(glm::vec3(-rotations.y, 0.0f, 0.0f));
		glm::quat yaw = glm::quat(glm::vec3(0.0f, -rotations.x, 0.0f));

		m_rotation = glm::normalize(yaw * m_rotation * pitch);

		glm::vec3 camera_roll_direction = m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 camera_pitch_direction = m_rotation * glm::vec3(-1.0f, 0.0f, 0.0f);

		// forward/backward move - all axes could be affected
		m_position += directions[0] * camera_roll_direction * dt * 50.0f;
		// left and right strafe - only xz could be affected    
		m_position += directions[1] * camera_pitch_direction * dt * 50.0f;
		// up and down flying - only y-axis could be affected
		m_position.y += directions[2] * dt * 50.0f;

		m_viewMatrix = glm::lookAt(m_position, m_position + camera_roll_direction,
			glm::cross(camera_roll_direction, camera_pitch_direction));
	}
}
