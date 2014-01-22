#ifndef SRC_CORE_SYSTEMS_RIOTERDATASYSTEM_H
#define SRC_CORE_SYSTEMS_RIOTERDATASYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	class RioterDataSystem : public BaseSystem
	{
	public:
		RioterDataSystem();
		virtual void Update( float delta ) override;

        int GetRioterInGroupCount( int groupid );
        std::vector<Entity> GetRiotersInGroup( int groupid );

        
		virtual const char* GetHumanName(){ return "RioterDataSystem"; }

	};
}
#endif
