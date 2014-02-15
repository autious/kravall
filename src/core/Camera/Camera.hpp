#ifndef SRC_CORE_CAMERA_CAMERA_HPP
#define SRC_CORE_CAMERA_CAMERA_HPP

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace Core
{
	/*!
	A class which is used to represent a camera. 
	Constructs matrices for use in rendering
	*/
	class Camera
	{
	public:
        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;

        void SetViewMatrix( const glm::mat4 & view );
        void SetProjectionMatrix( const glm::mat4 & proj, float nearZ, float farZ );

        glm::vec3 GetForward() const;
        glm::vec3 GetUp() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetPosition() const;
		
		float GetNear() const;
		float GetFar() const;

    private:
		glm::mat4x4 m_viewMatrix;
		glm::mat4x4 m_projectionMatrix;
		float m_nearZ;
		float m_farZ;

	};

	/*!
		The camera for the game, should not be manipulated by systems, only via lua.
		Initialized in main.
	*/
	extern Camera gameCamera;
    extern Camera overlayCamera;
}
#endif
