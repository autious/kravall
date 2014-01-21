#include "RioterDataSystem.hpp"

#include <SystemDef.hpp>
#include <World.hpp>
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>

namespace Core
{
		RioterDataSystem::RioterDataSystem() 
			: BaseSystem(EntityHandler::GenerateAspect<AttributeComponent,UnitTypeComponent>(), 0ULL)
		{
		}

	    void RioterDataSystem::Update(float delta) 
        {
        }

        int RioterDataSystem::GetRioterInGroupCount( int groupid )
        {
            int count = 0;
            for (std::vector<Entity>::iterator it = m_entities.begin();
                it != m_entities.end();
                it++)
            {
                AttributeComponent *ac = WGETC<AttributeComponent>( *it );  
                UnitTypeComponent *utc = WGETC<UnitTypeComponent>( *it );

                if( ac->rioter.groupID == groupid && utc->type == Core::UnitType::Rioter )
                    count++;
            }
        
            return count;
        }

        std::vector<Entity> RioterDataSystem::GetRiotersInGroup( int groupid )
        {
            std::vector<Entity> ents;
            for (std::vector<Entity>::iterator it = m_entities.begin();
                it != m_entities.end();
                it++)
            {
                AttributeComponent *ac = WGETC<AttributeComponent>( *it );  
                UnitTypeComponent *utc = WGETC<UnitTypeComponent>( *it );

                if( ac->rioter.groupID == groupid && utc->type == Core::UnitType::Rioter )
                    ents.push_back( *it );
            }

            return ents;
        }
}
