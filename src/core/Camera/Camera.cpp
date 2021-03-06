#include "Camera.hpp"
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
	Camera gameCamera;
    Camera overlayCamera;

    const glm::mat4& Camera::GetProjectionMatrix() const 
    {
        return m_projectionMatrix;
    }

    const glm::mat4& Camera::GetViewMatrix() const
    {
        return m_viewMatrix;
    }

    void Camera::SetViewMatrix( const glm::mat4 & view )
    {
        m_viewMatrix = view;
    }

    void Camera::SetProjectionMatrix( const glm::mat4 & proj )
    {
        m_projectionMatrix = proj;
		m_nearZ = proj[3][2] / (proj[2][2] - 1.0);
		m_farZ = proj[3][2] / (proj[2][2] + 1.0);
    }

    glm::vec3 Camera::GetForward() const
    {
		return glm::vec3( -m_viewMatrix[0][2], -m_viewMatrix[1][2], -m_viewMatrix[2][2] );
    }
    
    glm::vec3 Camera::GetUp() const
    {
		return glm::vec3( m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1] );
    }

    glm::vec3 Camera::GetRight() const
    {
        return glm::vec3( m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0] );
    }

    glm::vec3 Camera::GetPosition() const 
    {
        //glm::vec4 f = m_viewMatrix * glm::vec4( 0,0,0,1);
		glm::mat4 inv = glm::inverse( m_viewMatrix );
        //return glm::vec3( f[0],f[1],f[2] );
		return glm::vec3( inv[3][0], inv[3][1], inv[3][2] );
    }

	float Camera::GetNear() const
	{
		return m_nearZ;
	}

	float Camera::GetFar() const
	{
		return m_farZ;
	}

    float Camera::GetYaw() const
    {
        glm::vec4 forward_p = m_viewMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        forward_p = glm::normalize(glm::vec4(forward_p.x, forward_p.y, 0.0f, 0.0f));

        if(forward_p.x == 0.0f && forward_p.y == 0.0f)
        {
           if(forward_p.z > 0.0f)
           {
                return 3.14f;
           }
           else           
           {
                return 0.0f;
           }
        }
        return glm::atan(forward_p.y, forward_p.x);
    }

    float Camera::GetPitch() const
    {
        glm::vec4 forward_p = m_viewMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

        if(forward_p.y == 0.0f && forward_p.z == 0.0f)
        {
           if(forward_p.x > 0.0f)
           {
                return (3.14f/2.0f);
           }
           else           
           {
                return -(3.14f/2.0f);
           }
        }

        forward_p = glm::normalize(glm::vec4(0.0f, forward_p.y, forward_p.z, 0.0f));
        return -glm::atan(forward_p.y / forward_p.z);
    }

	void Camera::LookAt(const glm::vec3& position, const glm::vec3& target)
	{
        glm::mat4 viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
        SetViewMatrix(viewMatrix);
	}

    /*
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
    */
}
