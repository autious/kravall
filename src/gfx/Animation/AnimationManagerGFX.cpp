#include "AnimationManagerGFX.hpp"

AnimationManagerGFX::AnimationManagerGFX()
{
	glGenBuffers(1, &m_animationBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_animationBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_FRAMES * sizeof(glm::mat4x4), NULL, GL_STREAM_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_animationBuffer);
	m_idCounter = 0;
}

AnimationManagerGFX::~AnimationManagerGFX()
{
	glDeleteBuffers(1, &m_animationBuffer);
	m_animations.clear();
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
	if (numFrames > 0 && numBonesPerFrame > 0 && m_animations.size() < MAX_FRAMES)
	{
		if (m_skeletons.find(skeletonID) != m_skeletons.end())
		{
			int result = m_skeletons[skeletonID]->AddAnimationInfo(m_animations.size(), numFrames, numBonesPerFrame);
			if (result >= 0)
			{
				for (int i = 0; i < numFrames * numBonesPerFrame; i++)
				{
					m_animations.push_back(frames[i]);
				}
			}
			return result;
		}
		else
			return GFX_INVALID_SKELETON;
	}
	else
	{
		if (numFrames <= 0)
			return GFX_INVALID_NR_FRAMES;
		else if (numBonesPerFrame <= 0)
			return GFX_INVALID_NR_BONES;
		else if (m_animations.size() > MAX_FRAMES)
			return GFX_INVALID_MAX_FRAMES;
		else
			return GFX_INVALID_ANIMATION;
	}
}

int AnimationManagerGFX::GetFrameInfo(const int& skeletonID, const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset)
{
	if (m_skeletons.find(skeletonID) != m_skeletons.end())
	{
		return m_skeletons[skeletonID]->GetInfo(animationID, out_frameCount, out_bonesPerFrame, out_animationOffset);
	}
	return GFX_INVALID_SKELETON;
}


void AnimationManagerGFX::BindBufferData()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_animationBuffer);
	
	glm::mat4x4* pData = (glm::mat4x4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_FRAMES * sizeof(glm::mat4x4), 
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	
	memcpy(pData, m_animations.data(), m_animations.size() * sizeof(glm::mat4x4));
	
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
