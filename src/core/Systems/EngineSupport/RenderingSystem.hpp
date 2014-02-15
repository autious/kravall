#ifndef SRC_CORE_SYSTEMS_RENDERING_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_RENDERING_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	class RenderingSystem : public BaseSystem
	{
	public:
        RenderingSystem();
		virtual void Update(float delta) override;


		virtual const char* GetHumanName(){ return "RenderingSystem"; }
	};
}

#endif
