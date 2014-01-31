#include "AnimationManager.hpp"

AnimationManager::AnimationManager()
{
	m_idCounter = 0;
}

AnimationManager::~AnimationManager()
{

}

int AnimationManager::CreateSkeleton(int& out_skeletonID)
{
	out_skeletonID = m_idCounter++;
	m_skeletons[out_skeletonID] = new Skeleton();
	return GFX_SUCCESS;
}

int AnimationManager::DeleteSkeleton(const int& skeletonID)
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

int AnimationManager::AddAnimationToSkeleton(const int& skeletonID, glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->AddAnimation(frames, numFrames, numBonesPerFrame);
	}
	else
		return GFX_INVALID_SKELETON;
}

int AnimationManager::GetFrameInfo(const int& skeletonID, const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->GetInfo(animationID, out_frameCount, out_bonesPerFrame);
	}
	return GFX_INVALID_SKELETON;
}


void AnimationManager::BindSkeletonData(const int& skeletonID)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		m_skeletons[skeletonID]->BindBuffersData();
	}
}

void AnimationManager::BindSkeleton(const int& skeletonID)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		m_skeletons[skeletonID]->BindBuffers();
	}
}