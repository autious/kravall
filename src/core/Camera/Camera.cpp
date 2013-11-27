#include "Camera.hpp"
#include <algorithm>

namespace Core
{

	Camera::Camera(const float& fov, const float& nearZ, const float& farZ)
	{
		m_fov = fov;
		m_near = nearZ;
		m_far = farZ;
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

		m_viewMatrix = glm::mat4x4(1.0f);
		m_projectionMatrix = glm::mat4x4(1.0f);
	}

	Camera::~Camera()
	{

	}

	void Camera::LookAt(glm::vec3 lookAtTarget, glm::vec3 up)
	{
		// TODO: Verify
		glm::vec3 vz = glm::normalize(m_position - lookAtTarget);
		glm::vec3 vx = glm::normalize(glm::cross(up, vz));
		glm::vec3 vy = glm::cross(vz, vx);
		m_viewMatrix =
			glm::mat4x4(
			vx.x,						vy.x,						vz.x,						0.0f,
			vx.y,						vy.y,						vz.y,						0.0f,
			vx.z,						vy.z,						vz.z,						0.0f,
			glm::dot(vx, -m_position),	glm::dot(vy, -m_position),	glm::dot(vz, -m_position),	1.0f
			);

		m_rotationXYZ.x = std::atan2( m_viewMatrix[3][2], m_viewMatrix[3][3] );
		m_rotationXYZ.y = std::atan2( -m_viewMatrix[3][1], 
			sqrt(m_viewMatrix[3][2] * m_viewMatrix[3][2] + m_viewMatrix[3][3] * m_viewMatrix[3][3]) );
		m_rotationXYZ.z = std::atan2(m_viewMatrix[2][1], m_viewMatrix[1][1]);
	}

	void Camera::CalculateViewMatrix()
	{
		// TODO: Verify

		// Create temporary matrices for use in calculation
		glm::mat4x4 rotationMatrix = glm::mat4x4(1.0f);
		glm::mat4x4 translationMatrix = glm::mat4x4(1.0f);

		// Construct rotation matrix
		rotationMatrix *= glm::axisAngleMatrix(glm::vec3(1.0f, 0.0f, 0.0f), m_rotationXYZ.x);
		rotationMatrix *= glm::axisAngleMatrix(glm::vec3(0.0f, 1.0f, 0.0f), m_rotationXYZ.y);
		rotationMatrix *= glm::axisAngleMatrix(glm::vec3(0.0f, 0.0f, 1.0f), m_rotationXYZ.z);

		// Construct translation matrix
		translationMatrix = glm::translate(m_position);

		// Sum up the result
		m_viewMatrix = translationMatrix * rotationMatrix;

	}

	void Camera::CalculateProjectionMatrix(const unsigned int &width, const unsigned int &height)
	{
		// TODO: Verify
		m_projectionMatrix = glm::perspectiveFov(m_fov, static_cast<float>(width), 
			static_cast<float>(height), m_near, m_far);
	}

	void Camera::CalculateProjectionOrtho(const unsigned int &width, const unsigned int &height)
	{
		// TODO: Verify
		unsigned int halfWidth = width / 2;
		unsigned int halfHeight = height / 2;
		m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight);
	}


	float* Camera::GetProjectionMatrixAsArray()
	{
		return (float*)glm::value_ptr(m_projectionMatrix);
	}

	float* Camera::GetViewMatrixAsArray()
	{
		return (float*)glm::value_ptr(m_viewMatrix);
	}

}