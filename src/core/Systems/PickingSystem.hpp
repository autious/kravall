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
			If no hit entity max is returned.
		*/
		Entity GetHitEntity( int mouseX, int mouseY );

		/*!
			Will check a ray from the game camera via cursor to far-plane vs. the XZ plane.
		*/
		glm::vec3 GetGroundHit( int mouseX, int mouseY );

		/*!
			Will return the current intersection of the mouse ray and X-Z plane.
		*/
		glm::vec3 GetCurrentGroundHit() { return m_currentGroundHit; }


		/*!
			Will return a ray going from the game camera via cursor to far-plane.
		*/
		glm::vec3 GetRayFromCamera( int mouseX, int mouseY );

		/*!
			Returns the selected entity, if any. Otherwise, entity max is returned.
		*/
		Entity GetLastHitEntity();

	private:

		Entity m_lastSelectedEntity;
		glm::vec3 m_currentGroundHit;
    };
}

#endif
