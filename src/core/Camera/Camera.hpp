#ifndef SRC_CORE_CAMERA_CAMERA_HPP
#define SRC_CORE_CAMERA_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Core
{
	/*!
	A class which is used to represent a camera. 
	Constructs matrices for use in rendering
	*/
	class Camera
	{
	public:
		/*!
		Constructor.
		*/
		Camera(const float& fov, const float& near, const float& far);

		/*!
		Destructor.
		*/
		~Camera();

		/*!
		Recalculates the camera rotation to look at the specified target position.
		\param lookAtTarget The position to look at
		\param up The camera up vector
		*/
		void LookAt(glm::vec3 lookAtTarget, glm::vec3 up);

		/*!
		Gets the camera position.
		\return Returns a vector containing the world space position of the camera
		*/
		inline glm::vec3 GetPosition() const { return m_position; }

		/*!
		Sets the camera position.
		\param position The position to place the camera at
		*/
		inline void SetPosition(const glm::vec3& position){ m_position = position; }

		/*!
		Gets the camera rotation in global space.
		\return Returns the camera rotation in global space as a vector
		*/
		inline glm::vec3 GetRotationXYZ() const { return m_rotationXYZ; }

		/*!
		Sets the camera rotation in global space.
		\param rotXYZ The rotation in global space to apply to the camera
		*/
		inline void SetRotationXYZ(const glm::vec3& rotXYZ){ m_rotationXYZ = rotXYZ; }

		/*!
		Recalculates the camera view matrix.
		*/
		void CalculateViewMatrix();

		/*!
		Recalculates the camera projection matrix.
		\param width Window width
		\param height Window height
		*/
		void CalculateProjectionMatrix(const unsigned int &width, const unsigned int &height);

		/*!
		Recalculates the camera projection matrix as an orthogonal projection.
		\param width Window width
		\param height Window height
		*/
		void CalculateProjectionOrtho(const unsigned int &width, const unsigned int &height);

		/*!
		Gets the view matrix representing the camera. Note that calling this function
		does not automatically update the view matrix.
		\return Returns the view matrix.
		*/
		inline glm::mat4x4 GetViewMatrix() const { return m_viewMatrix; }

		/*!
		Gets the projection matrix representing the camera lens. Note that calling this function 
		does not automatically update the projection matrix.
		\return Returns the projection matrix.
		*/
		inline glm::mat4x4 GetProjectionMatrix() const { return m_projectionMatrix; }

		/*!
		Sets the camera field of view.
		\param fov The Field of view in degrees
		*/
		inline void SetFov(const float& fov) { m_fov = fov; }

		/*!
		Sets the near and far clipping plane of the camera.
		\return Returns the projection matrix.
		*/
		inline void SetNearFar(const float& near, const float& far) { m_near = near; m_far = far; }


	private:

		glm::vec3 m_position;
		glm::vec3 m_rotationXYZ;

		glm::mat4x4 m_viewMatrix;
		glm::mat4x4 m_projectionMatrix;

		float m_near;
		float m_far;
		float m_fov;

	};
}
#endif