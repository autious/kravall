#include "AnimationManagerGFX.hpp"

AnimationManagerGFX::AnimationManagerGFX()
{
	m_idCounter = 0;
}

AnimationManagerGFX::~AnimationManagerGFX()
{

}

int AnimationManagerGFX::CreateSkeleton(int& out_skeletonID)
{
	out_skeletonID = m_idCounter++;
	m_skeletons[out_skeletonID] = new Skeleton();
	return GFX_SUCCESS;
}

int AnimationManagerGFX::DeleteSkeleton(const int& skeletonID)
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

int AnimationManagerGFX::AddAnimationToSkeleton(const int& skeletonID, glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->AddAnimation(frames, numFrames, numBonesPerFrame);
	}
	else
		return GFX_INVALID_SKELETON;
}

int AnimationManagerGFX::GetFrameInfo(const int& skeletonID, const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->GetInfo(animationID, out_frameCount, out_bonesPerFrame, out_animationOffset);
	}
	return GFX_INVALID_SKELETON;
}


void AnimationManagerGFX::BindSkeletonData(const int& skeletonID)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		m_skeletons[skeletonID]->BindBuffersData();
	}
}

void AnimationManagerGFX::BindSkeleton(const int& skeletonID)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		m_skeletons[skeletonID]->BindBuffers();
	}
}
