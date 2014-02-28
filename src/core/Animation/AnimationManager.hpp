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
		
		// Interface functions, exposed to lua
		static void PlayAnimation(const Entity& entity, const std::string& animationName);				// exists in Lua
		static void PlayAnimation(const Entity& entity, int animationID );
		static void LoopAnimation(const Entity& entity, const std::string& animationName);				// exists in Lua
		static void LoopAnimation(const Entity& entity, int animationID );	
		static void QueueAnimation(const Entity& entity, const std::string& animationName, bool loop);
		static void QueueAnimation(const Entity& entity, const int animationID, bool loop);
		static void PauseAnimation(const Entity& entity);												// exists in lua
		static void ResumeAnimation(const Entity& entity);												// exists in lua
		static void SetAnimationSpeed(const Entity& entity, float speed);								// exists in lua
		static void SetAnimationTime(const Entity& entity, float time);									// exists in lua

        static bool IsAnimationPlaying( const Entity& entity );

		static int GetAnimationID(int meshID, const std::string& animationName);
		static int StoreAnimationID(int meshID, int animationID, const std::string& animationName);

	private:

		static std::map<int, std::map<std::string, int> > m_skeletons;
	};
}
#endif
