#ifndef SRC_CORE_SYSTEMS_LIGHT_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_LIGHT_SYSTEM_HPP

#include <SystemDef.hpp>

namespace Core
{
	class LightSystem : public BaseSystem
	{
	public:
		virtual void Update(float delta) override;

		LightSystem() :BaseSystem(EntityHandler::GenerateAspect<LightComponent, WorldPositionComponent, RotationComponent, ScaleComponent>(), 0ULL)
		{}

		virtual const char* GetHumanName(){ return "RenderingSystem"; }
	};
}

#endif