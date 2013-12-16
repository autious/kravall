#ifndef BOUNDING_VOLUME_COMPONENT_HPP
#define BOUNDING_VOLUME_COMPONENT_HPP

namespace Core
{
	/*!
		Enum identifying the collision volume and layout of the data.
	*/
	enum BoundingVolumeType
	{
		/*! 
			If specified, entety will be checked using a sphere volume and the data in the BoundingVolumeComponent will be interpreted accordingly. 
		*/
		SphereBoundingType, 
		
		/*! 
			If specified, entety will be checked using a AABB volume and the data in the BoundingVolumeComponent will be interpreted accordingly. 
		*/
		AABBBoundingType, 

		/*!
			Default type of the BoundingVolumeComponent, will exclude the component from calculations.
		*/
		NoBoundingVolume,
	};


	/*!
		Data container for Sphere type bounding volumes.
	*/
	struct BoundingSphere
	{
		/*! 
			Radius of the spehere. 
		*/
		float radius;
		
		/*! 
			Vector members to the middle of the object in object space. 
		*/
		float offset[3];
		
		/*!
			Static function used to convert the offset[3] to a glm::vec3 pointer.
		*/
		static glm::vec3* GetVec3( float* offset )
		{
			return (glm::vec3*)(offset);
		}

		/*! 
			Default constructor initializes all members to zero.
		*/
		BoundingSphere()
			: radius( 0.0f )
		{
			offset[0] = 0;
			offset[1] = 0;
			offset[2] = 0;
		}
			
		/*!
			\param radius Radius of the sphere.
			\param offsetX X offset of the sphere from the object.
			\param offsetY Y offset of the sphere from the object.
			\param offsetZ Z offset of the sphere from the object.
		*/
		BoundingSphere( float radius, float offsetX, float offsetY, float offsetZ )
			: radius( radius )
		{
			offset[0] = offsetX;
			offset[1] = offsetY;
			offset[2] = offsetZ;
		}
			
	};

	/*!
		Data container for AABB type volumes.
	*/
	struct AABB
	{
		/*!
			Vector to the middle of the object in object space. 
		*/
		float offset[3];

		/*!
			X, Y and Z axis aligned half widths of the AABB. 
		*/
		float halfWidths[3]; 

		/*! 
			Do nothing constructor.
		*/
		AABB()
		{
		}

	};

#define MAX_SIZE_OF_BOUNDING_STRUCTS 6 * 4


	/*!
		Contains bounding volume data for the respective entity.
	*/
	struct BoundingVolumeComponent
	{
		/*!
			Hint for how to interpret the m_data member.
		*/
		BoundingVolumeType type;

		// TO DO : add comment here
		char data[ MAX_SIZE_OF_BOUNDING_STRUCTS ];

		/*!
			Default constructor, will set type to NoVolume.
		*/
		BoundingVolumeComponent()
		{
			type = Core::BoundingVolumeType::NoBoundingVolume;
		}

		/*!
			Constructor initializing the component to a SphereBoundingType.
			\param sphere Sphere container with respective data.
		*/
		BoundingVolumeComponent( Core::BoundingSphere sphere )
		{
			type = Core::BoundingVolumeType::SphereBoundingType;
			std::memcpy( &data, &sphere,  MAX_SIZE_OF_BOUNDING_STRUCTS );
		}

		/*!
			Constructor initializing the component to a AABBBoundingType.
			\param aabb AABB container with respective data.
		*/
		BoundingVolumeComponent( Core::AABB aabb )
		{
			type = Core::BoundingVolumeType::AABBBoundingType;
			std::memcpy( &data, &aabb,  MAX_SIZE_OF_BOUNDING_STRUCTS );
		}


	};
}
#endif