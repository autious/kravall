#ifndef SRC_CORE_SYSTEMS_LIGHT_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_LIGHT_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	class LightSystem : public BaseSystem
	{
	public:
		virtual void Update(float delta) override;

		LightSystem();

		virtual const char* GetHumanName(){ return "LightSystem"; }
	};
}

#endif