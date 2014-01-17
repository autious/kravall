#ifndef CORE_SYSTEMS_GROUP_MEDIAN_POSITION_SYSTEM_HPP
#define CORE_SYSTEMS_GROUP_MEDIAN_POSITION_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

#include <glm/glm.hpp>

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

        inline glm::vec3 GetMedianPosition( int groupId ) { return m_medianPositions[groupId]; }
        inline int GetMemberCount( int groupId ) { return m_groupMemberCounts[groupId]; }
        inline int GetNumberOfGroups( int groupId ) { return m_nAllocatedGroups; }

    private:
        void CalculateMedianPositions();

        int m_nAllocatedGroups;
        glm::vec3* m_medianPositions;
        int* m_groupMemberCounts;
    };
}

#endif
