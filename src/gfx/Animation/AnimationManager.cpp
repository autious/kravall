#include "AnimationManager.hpp"

AnimationManager::AnimationManager()
{
	m_idCounter = 0;
}

AnimationManager::~AnimationManager()
{

}

int AnimationManager::CreateSkeleton(unsigned int& out_skeletonID)
{
	out_skeletonID = m_idCounter++;
	m_skeletons[out_skeletonID] = new Skeleton();
	return GFX_SUCCESS;
}

int AnimationManager::DeleteSkeleton(const unsigned int& skeletonID)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		delete m_skeletons[skeletonID];
		m_skeletons.erase(skeletonID);
		return GFX_SUCCESS;
	}
	else
		return GFX_FAIL;
}

int AnimationManager::AddAnimationToSkeleton(const unsigned int& skeletonID, glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->AddAnimation(frames, numFrames, numBonesPerFrame);
	}
	else
		return GFX_INVALID_SKELETON;
}

int AnimationManager::GetFrameCount(const unsigned int& skeletonID, const unsigned int& animationID, unsigned int& out_frameCount)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->GetFrameCount(animationID, out_frameCount);
	}
	return GFX_INVALID_SKELETON;
}