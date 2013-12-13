#ifndef SRC_CORE_COMPONENTS_ROTATION_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_ROTATION_COMPONENT_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
namespace Core
{
	struct RotationComponent
	{
		float rotation[4];

		RotationComponent()
		{
			rotation[0] = 0.0f;
			rotation[1] = 0.0f;
			rotation[2] = 0.0f;
			rotation[3] = 1.0f;
		}

		RotationComponent(float x, float y, float z, float w)
		{
			rotation[0] = x;
			rotation[1] = y;
			rotation[2] = z;
			rotation[3] = w;
		}

		static glm::quat GetQuat(float* rotation)
		{
			glm::quat q;
			q.x = rotation[0];
			q.y = rotation[1];
			q.z = rotation[2];
			q.w = rotation[3];
			
			return q;
		}

		/*! 
			Utility function for creating a RotationComponent with the specified angle of rotation around the x-axis.
			\param angleRad The angle in radians to rotate.
			\return A RotationComponent with its members set to the specified rotation.
		*/
		static RotationComponent GetComponentRotateX(float angleRad)
		{
			return GetComponentRotateAxis(angleRad, 1.0f, 0.0f, 0.0f);
		}

		/*!
			Utility function for creating a RotationComponent with the specified angle of rotation around the y-axis.
			\param angleRad The angle in radians to rotate.
			\return A RotationComponent with its members set to the specified rotation.
		*/
		static RotationComponent GetComponentRotateY(float angleRad)
		{
			return GetComponentRotateAxis(angleRad, 0.0f, 1.0f, 0.0f);
		}

		/*!
			Utility function for creating a RotationComponent with the specified angle of rotation around the z-axis.
			\param angleRad The angle in radians to rotate.
			\return A RotationComponent with its members set to the specified rotation.
		*/
		static RotationComponent GetComponentRotateZ(float angleRad)
		{
			return GetComponentRotateAxis(angleRad, 0.0f, 0.0f, 1.0f);
		}

		/*!
			Utility function for creating a RotationComponent with the specified angle of rotation around an arbitrary
			axis.
			\param angleRad The angle in radians to rotate.
			\param axisX The x-component of the vector specifying the rotation axis.
			\param axisY The y-component of the vector specifying the rotation axis.
			\param axisZ The z-component of the vector specifying the rotation axis.
			\return A RotationComponent with its members set to the specified rotation.
		*/
		static RotationComponent GetComponentRotateAxis(float angleRad, float axisX, float axisY, float axisZ)
		{
			RotationComponent rc;
			rc.rotation[0] = axisX * std::sin(angleRad * 0.5);
			rc.rotation[1] = axisY * std::sin(angleRad * 0.5);
			rc.rotation[2] = axisZ * std::sin(angleRad * 0.5);
			rc.rotation[3] = std::cos(angleRad * 0.5);

			return rc;
		}
	};
}

#endif
