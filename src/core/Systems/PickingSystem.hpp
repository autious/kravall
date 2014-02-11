#ifndef SRC_CORE_SYSTEMS_PICKINGSYSTEM_HPP
#define SRC_CORE_SYSTEMS_PICKINGSYSTEM_HPP

#include <SystemDef.hpp>

namespace Core
{
	/*!
		Passive system that does nothing at Update() but hosts functionality for picking bounding volumes. 
		Get and cast the pointer to the system via the systemHandler. 
	*/
    class PickingSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        PickingSystem();

        virtual const char* GetHumanName() { return "PickingSystem"; }

		/*!
			Will check a ray from the game camera via cursor to far-plane vs. eligable enteties.
			The recorded hit will be the closest from origin.
			Will also set the entity as selected and gettable via the GetLastHitEntity() function.
			If no entity is hit INVALID_ENTITY is returned.
		*/
		Entity GetHitEntity(int mouseX, int mouseY, Core::Aspect aspectMask = 0ULL);

		/*!
			Will check a ray from the game camera via cursor to far-plane vs. the XZ plane.
		*/
		glm::vec3 GetGroundHit( int mouseX, int mouseY );

		/*!
			Will return the current intersection of the mouse ray and X-Z plane.
		*/
		glm::vec3 GetCurrentGroundHit() { return m_currentGroundHit; }

		/*!
			Will form a frustum form the 2 screen space points and return a list of all entities that 
			graceDistance is how far outside the frustum the object center is allowed to be. Can be negative.
		*/
		std::vector< Core::Entity > BoxSelect( int X, int Y, int X2, int Y2, float graceDistance, Core::Aspect aspectMask = 0ULL );

		/*!
			Will return a ray going from the game camera via cursor to far-plane.
		*/
		glm::vec3 GetRayFromCamera( int mouseX, int mouseY );

		/*!
			Returns the selected entity, if any. Otherwise INVALID_ENTITY is returned.
		*/
		inline Entity GetLastHitEntity()
        {
            return m_lastSelectedEntity;
        }

	private:

		Entity m_lastSelectedEntity;
		glm::vec3 m_currentGroundHit;
    };
}

#endif
