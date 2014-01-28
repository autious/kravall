#include "AnimationManager.hpp"
#include <logger/Logger.hpp>
#include <gfx/GFXInterface.hpp>
#include <World.hpp>

namespace Core
{
	std::vector< std::map<std::string, int> > AnimationManager::m_skeletons;

	void AnimationManager::PlayAnimation(const Entity& entity, const std::string& animationName)
	{
		Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(entity);
		if (ac != nullptr)
		{
			int skeletonID = ac->skeletonID;
			int animationID = GetAnimationID(skeletonID, animationName);
			if (skeletonID < 0 || animationID < 0)
			{
				LOG_WARNING << "Failed to play animation \'" << animationName << "\': Cannot find animation";
			}
			else
			{
				ac->animationID = animationID;
				ac->currentTime = 0.0;
				ac->playing = true;
				ac->loop = false;
			}
		}
		else
		{
			LOG_WARNING << "Failed to play animation \'" << animationName << "\': No animation component attached to entity";
		}
	}

	void AnimationManager::LoopAnimation(const Entity& entity, const std::string& animationName)
	{
		Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(entity);
		if (ac != nullptr)
		{
			int skeletonID = ac->skeletonID;
			int animationID = GetAnimationID(skeletonID, animationName);
			if (skeletonID < 0 || skeletonID > (int)m_skeletons.size() || animationID < 0)
			{
				LOG_WARNING << "Failed to loop animation \'" << animationName << "\': Cannot find animation";
			}
			else
			{
				ac->animationID = animationID;
				ac->currentTime = 0.0;
				ac->playing = true;
				ac->loop = true;
			}
		}
		else
		{
			LOG_WARNING << "Failed to loop animation \'" << animationName << "\': No animation component attached to entity";
		}
	}

	void AnimationManager::HaltAnimation(const Entity& entity)
	{
		Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(entity);
		if (ac != nullptr)
		{
			ac->playing = false;
		}
		else
		{
			LOG_WARNING << "Failed to halt animation: No animation component attached to entity";
		}
	}

	int AnimationManager::GetAnimationID(int skeletonID, std::string animationName)
	{
		if (m_skeletons[skeletonID].find(animationName) != m_skeletons[skeletonID].end())
			return m_skeletons[skeletonID][animationName];
		else
			return -1;
	}
	
	void AnimationManager::SetAnimationSpeed(const Entity& entity, float speed)
	{
		Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(entity);
		if (ac != nullptr)
		{
			ac->speed = speed;
		}
		else
		{
			LOG_WARNING << "Failed to set animation speed: No animation component attached to entity";
		}
	}
}