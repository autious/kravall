#ifndef CORE_SYSTEMS_GROUP_MEDIAN_POSITION_SYSTEM_HPP
#define CORE_SYSTEMS_GROUP_MEDIAN_POSITION_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

#include <glm/glm.hpp>

#include <vector>

namespace Core 
{
    /*!
        Game logic system that calculates various data regarding groups of entities.
    */
    class GroupDataSystem : public  Core::BaseSystem
    {
    public:
        GroupDataSystem();

        virtual void Update( float delta ) override;

        virtual const char* GetHumanName() { return "GroupDataSystem"; }

        inline glm::vec3 GetMedianPosition( int groupId ) 
        { 
            if(m_medianPositions)
            {
                return m_medianPositions[groupId]; 
            }
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }

        inline int GetMemberCount( int groupId ) 
        { 
            if(m_groupMemberCounts)
            {
               return m_groupMemberCounts[groupId]; 
            }
            return 0;
        }
        
        inline int GetNumberOfGroups() { return m_nAllocatedGroups; }

        std::vector<Core::Entity> GetMembersInGroup(int groupId);

    private:
        void CalculateMedianPositions();
        void CalculateMembersInGroups();

        std::vector<std::vector<Core::Entity>> m_membersInGroups;
        int m_nAllocatedGroups;
        glm::vec3* m_medianPositions;
        int* m_groupMemberCounts;
    };
}

#endif
