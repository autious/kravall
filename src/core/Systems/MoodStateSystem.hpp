#ifndef SRC_CORE_SYSTEMS_MOOD_STATE_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_MOOD_STATE_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	class MoodStateSystem : public BaseSystem
	{
	public:
		MoodStateSystem();
		virtual void Update(float delta) override;


		virtual const char* GetHumanName(){ return "RioterStateSystem"; }

	private:
	};
}

#endif
