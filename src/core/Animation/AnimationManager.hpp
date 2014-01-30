#ifndef CORE_ANIMATION_ANIMATION_MANAGER_HPP
#define CORE_ANIMATION_ANIMATION_MANAGER_HPP

#include <string>
#include <map>
#include <vector>
#include <ComponentFramework/SystemTypes.hpp>
namespace Core
{
	class AnimationManager
	{
	public:
		
		static void PlayAnimation(const Entity& entity, const std::string& animationName);
		static void LoopAnimation(const Entity& entity, const std::string& animationName);
		static void HaltAnimation(const Entity& entity);
		static void SetAnimationSpeed(const Entity& entity, float speed);
		static int GetAnimationID(int meshID, const std::string& animationName);
		static int StoreAnimationID(int meshID, int animationID, const std::string& animationName);

	private:

		static std::vector<std::map<std::string, int> > m_skeletons;
	};
}
#endif