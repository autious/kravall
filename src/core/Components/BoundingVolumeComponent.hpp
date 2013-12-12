#ifndef ATTRIBUTE_RIOTER_COMPONENT_HPP
#define ATTRIBUTE_RIOTER_COMPONENT_HPP

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
		Contains bounding volume data for the respective entity.
	*/
	struct BoundingVolumeComponent
	{
		/*!
			Data container for Sphere type bounding volumes.
		*/
		struct BoundingSphere
		{
			/*! 
				Radius of the spehere. 
			*/
			float m_radius;
		
			/*! 
				Vector to the middle of the object in object space. 
			*/
			//float m_offset[3]; 

			/*! 
				Do nothing constructor.
			*/
			/*
			BoundingSphere()
			{
			}
			*/
			/*!
				\param radius Radius of the sphere.
				\param offsetX X offset of the sphere from the object.
				\param offsetY Y offset of the sphere from the object.
				\param offsetZ Z offset of the sphere from the object.
			*/
			/*
			BoundingSphere( float radius, float offsetX, float offsetY, float offsetZ )
			{
				/*
				m_radius = radius;
				m_offset[0] = offsetX;
				m_offset[2] = offsetY;
				m_offset[1] = offsetZ;
				
			}
			*/
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
		};



		/*!
			Hint for how to interpret the m_data union.
		*/
		BoundingVolumeType m_type;

		/*!
			Data container used to store information about all different types of bounding volumes. Should be accessed according to the BoundingVolumeType member.
		*/
		union
		{
			Core::BoundingVolumeComponent::BoundingSphere sphere;
			Core::BoundingVolumeComponent::AABB aabb;
		} m_data;

		/*!
			Default constructor, will set type to NoVolume.
		*/
		BoundingVolumeComponent()
		{
			m_type = Core::BoundingVolumeType::NoBoundingVolume;
		}

		/*!
			Constructor initializing the component to a SphereBoundingType.
			\param sphere Sphere container with respective data.
		*/
		BoundingVolumeComponent( Core::BoundingVolumeComponent::BoundingSphere sphere )
		{
			m_type = Core::BoundingVolumeType::SphereBoundingType;
			//m_data.sphere = sphere;
		}

		/*!
			Constructor initializing the component to a AABBBoundingType.
			\param aabb AABB container with respective data.
		*/
		BoundingVolumeComponent( Core::BoundingVolumeComponent::AABB aabb )
		{
			m_type = Core::BoundingVolumeType::AABBBoundingType;
			//m_data.aabb = aabb;
		}


	};
}
#endif