#ifndef SRC_CORE_SYSTEMS_RIOTER_STATE_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_RIOTER_STATE_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	class RioterStateSystem : public BaseSystem
	{
	public:
		RioterStateSystem();
		virtual void Update(float delta) override;


		virtual const char* GetHumanName(){ return "RioterStateSystem"; }

	private:
	};
}

#endif
