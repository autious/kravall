#ifndef SRC_CORE_SYSTEMS_REMOVAL_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_REMOVAL_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	class RemovalSystem : public BaseSystem
	{
	public:
		RemovalSystem();

		virtual void Update(float delta) override;


		virtual const char* GetHumanName(){ return "RemovalSystem"; }

	private:
	};
}

#endif
