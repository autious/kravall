#ifndef SRC_CORE_SYSTEMS_MOVEMENT_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_MOVEMENT_SYSTEM_HPP

#include <SystemDef.hpp>

namespace Core
{
	class MovementSystem : public BaseSystem
	{

	public:
		virtual void Update(float delta) override;

		MovementSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent>(), 0ULL)
		{}

		virtual const char* GetHumanName() { return "MovementSystem"; }
	};
}

#endif