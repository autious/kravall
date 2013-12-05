#ifndef SRC_CORE_SYSTEMS_RENDERING_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_RENDERING_SYSTEM_HPP

#include <SystemDef.hpp>

namespace Core
{
	class RenderingSystem : public BaseSystem
	{
	public:
		virtual void Update(float delta) override;

		RenderingSystem() :BaseSystem(EntityHandler::GenerateAspect<GraphicsComponent, WorldPositionComponent, RotationComponent, ScaleComponent>(), 0ULL)
		{}

		virtual const char* GetHumanName(){ return "RenderingSystem"; }
	};
}

#endif