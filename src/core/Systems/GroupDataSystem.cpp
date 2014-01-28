#include "GroupDataSystem.hpp"

#include <World.hpp>

#include <GameUtility/NavigationMesh.hpp>

#include <cstring>

#include <logger/Logger.hpp>

namespace Core
{
    GroupDataSystem::GroupDataSystem() : BaseSystem(EntityHandler::GenerateAspect<
            Core::WorldPositionComponent, UnitTypeComponent, AttributeComponent>(), 0ULL)
    {
        m_nAllocatedGroups = 0;
        m_medianPositions = nullptr;
        m_groupMemberCounts = nullptr;
    }

    void GroupDataSystem::Update(float delta)
    {
        Core::NavigationMesh* navMesh = Core::GetNavigationMesh();
        if(navMesh)
        {
            m_nAllocatedGroups = navMesh->nrUsedFlowfields;

            m_medianPositions = reinterpret_cast<glm::vec3*>(world.m_frameHeap.NewPODArray<unsigned char>(m_nAllocatedGroups * sizeof(glm::vec3)));
            memset(m_medianPositions, 0, sizeof(glm::vec3) * m_nAllocatedGroups);

            m_groupMemberCounts = world.m_frameHeap.NewPODArray<int>(m_nAllocatedGroups);
            memset(m_groupMemberCounts, 0, sizeof(int) * m_nAllocatedGroups);
            CalculateMedianPositions();
            CalculateMembersInGroups();
        }
        else
        {
            m_nAllocatedGroups = 0;
            m_medianPositions = nullptr;
            m_groupMemberCounts = nullptr;
            m_membersInGroups.clear();
        }
    }

    void GroupDataSystem::CalculateMedianPositions()
    {
        for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);

            if(utc->type == Core::UnitType::Rioter)
            {
                Core::AttributeComponent* atrbc = WGETC<Core::AttributeComponent>(*it);
                Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);

                if(atrbc->rioter.groupID >= 0 && atrbc->rioter.groupID <= m_nAllocatedGroups)
                {
                    m_medianPositions[atrbc->rioter.groupID] += Core::WorldPositionComponent::GetVec3(*wpc);
                    m_groupMemberCounts[atrbc->rioter.groupID]++;
                }
            }
        }

        for(int i=0; i < m_nAllocatedGroups; ++i)
        {
            if(m_groupMemberCounts[i])
            {
                m_medianPositions[i] = m_medianPositions[i] / static_cast<float>(m_groupMemberCounts[i]);
            }
        }
    }

    void GroupDataSystem::CalculateMembersInGroups()
    {
        m_membersInGroups.clear();

        for(int i=0; i < m_nAllocatedGroups; ++i)
        {
            m_membersInGroups.push_back(std::vector<Core::Entity>());
        }

        for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);

            if(utc->type == Core::UnitType::Rioter)
            {
                Core::AttributeComponent* atrbc = WGETC<Core::AttributeComponent>(*it);

                if( atrbc->rioter.groupID >= 0 && atrbc->rioter.groupID <= m_nAllocatedGroups)
                {
                    m_membersInGroups[atrbc->rioter.groupID].push_back(*it);
                }
            }
        }
    }

    std::vector<Core::Entity> GroupDataSystem::GetMembersInGroup(int groupId)
    {
        if(groupId >= 0 && groupId <= m_nAllocatedGroups)
        {
            return m_membersInGroups[groupId];
        }
        return std::vector<Core::Entity>();
    }
}
