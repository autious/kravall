#ifndef SRC_CORE_SYSTEMS_BLOOD_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_BLOOD_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

#include <gfx/BitmaskDefinitions.hpp>
#include <glm/glm.hpp>
namespace Core
{
	class BloodSystem : public BaseSystem
	{
	public:
		BloodSystem();

		virtual void Update(float delta) override;


		virtual const char* GetHumanName(){ return "BloodSystem"; }

	private:
		
	};
}

#endif
