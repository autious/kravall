#ifndef CORE_SYSTEMS_SQUAD_SYSTEM_HPP
#define CORE_SYSTEMS_SQUAD_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>
#include <Components/SquadComponent.hpp>
#include <glm/glm.hpp>

#include <gfx/BitmaskDefinitions.hpp>
namespace Core
{
    class SquadSystem : public Core::BaseSystem
    {
    public:
        SquadSystem();

        virtual void Update(float delta) override;
        virtual const char* GetHumanName() { return "SquadSystem"; }
        
        void SetSquadGoal(int* squadIDs, int nSquads, glm::vec3 target);

        /*!
            Sets a set of units' formation with the current paramters.
            \param squadIDs Pointer to an array of squad IDs.
            \param nSqauds The number of elements in array sent in to squadIDs.
            \param formation A SquadFormation enumeration that indicates which formation to place the units in.
            \param startPoint The point where the units will be placed from.
            \param endPoint Units will be placed between this point and startPoint.
        */
        void SetSquadFormation(int* squadIDs, int nSquads,  Core::SquadFormation formation, const glm::vec3& startPoint, const glm::vec3& endPoint);
        /*!
            Prints decals of where units would be placed if given a formation with the current paramters.
            \param squadIDs Pointer to an array of squad IDs.
            \param nSqauds The number of elements in array sent in to squadIDs.
            \param formation A SquadFormation enumeration that indicates which formation to place the units in.
            \param startPoint The point where the units will be placed from.
            \param endPoint Units will be placed between this point and startPoint.
        */
        void PreviewSquadFormation(int* squadIDs, int nSquads, Core::SquadFormation formation, const glm::vec3& startPoint, const glm::vec3& endPoint);

        /*!
            Sets the stances of the given squads.
            \param squadIDs Pointer to an array of squad IDs.
            \param nSqauds The number of elements in array sent in to squadIDs.
            \param stance The stance to set the squads to.
        */
        void SetSquadStance(int* squadIDs, int nSqauds, Core::PoliceStance stance);
       
        /*!
            Retrieves the entity with the given squadID. If there are more than one squad with the same ID the firt one encounterd is returned.
            \param squadID The ID of the entity to retrieve's Core::SquadComponent.
            \returns An entity with the given squadID, or INVALID_ENTITY if none is found.
        */
        Core::Entity GetSquadEntity(int squadID);

		/*!
			Will assign a target group for the assigned squad and set all members to attacking state.
			\param The id of the attacking squad.
			\param The id of the group to be attacked.
		*/
		void SetSquadTargetGroup( int sqadID, int targetSquadID );

		/*!
			Will revert the squads to their SquadComponent->prevSquadStance.
			\param squadIDs Pointer to an array of squad IDs.
            \param nSqauds The number of elements in array sent in to squadIDs.
			\returns the new stance for the first squad that is set.
		*/
		PoliceStance RevertSquadStanceFromAgressive( int* squadIDs, int nSquads );

        void EnableOutline(int* squadIDs, int nSqauds, const glm::vec4& Color);

		void EnableOutlineMoods(int* squadIDs, int nSquads);

        void DisableOutline(int* squadIDs, int nSqauds);

		/*!
			Will revert group stance if aggresive and set formation goal to current position.
		*/
		void StopGroup(int* squadIDs, int nSqauds);

        std::vector<int> GetAllSquads( );
        std::vector<Entity> GetAllSquadEntities();
        std::vector<Core::SquadAbility> GetPossibleAbilities( int squadId );

	private:
		void PushDecal(glm::vec3 position);
		bool m_foundAssets;
		int m_mesh;
		int m_material;

		GFX::GFXBitmask m_bitmask;
		GFX::GFXBitmask m_lightBitmask;
    };
}
#endif
