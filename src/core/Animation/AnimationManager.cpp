#include "AnimationManager.hpp"
#include <logger/Logger.hpp>
#include <gfx/GFXInterface.hpp>
#include <World.hpp>

namespace Core
{
	std::map<int, std::map<std::string, int> > AnimationManager::m_skeletons;

	void AnimationManager::PlayAnimation(const Entity& entity, const std::string& animationName)
	{
		Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(entity);
		Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(entity);
		if (ac != nullptr && gc != nullptr)
		{
			unsigned int meshID = GFX::GetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID);
			int animationID = GetAnimationID(meshID, animationName);
			if (animationID < 0)
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
		Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(entity);
		if (ac != nullptr && gc != nullptr)
		{
			unsigned int meshID = GFX::GetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID);
			int animationID = GetAnimationID(meshID, animationName);
			if (animationID < 0)
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

	int AnimationManager::GetAnimationID(int meshID, const std::string& animationName)
	{
		if (m_skeletons[meshID].find(animationName) != m_skeletons[meshID].end())
			return m_skeletons[meshID][animationName];
		else
			return -1;
	}

	int AnimationManager::StoreAnimationID(int meshID, int animationID, const std::string& animationName)
	{
		if (m_skeletons[meshID].find(animationName) == m_skeletons[meshID].end())
		{
			m_skeletons[meshID][animationName] = animationID;
			std::cout << "Stored animation \'" << animationName << "\' for mesh with id " << meshID << "\n";
			return 0;
		}
		else
		{
			LOG_WARNING << "Animation with name \'" << animationName << "\' already exists in mesh " << meshID;
			return -1;
		}
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