#include "Skeleton.hpp"

#include <cstring>

Skeleton::Skeleton()
{
	glGenBuffers(1, &m_animationBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_animationBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_FRAMES * sizeof(glm::mat4x4), NULL, GL_STREAM_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_animationBuffer);

	m_numFrames = 0;
	m_numAnimations = 0;
}

Skeleton::~Skeleton()
{
	glDeleteBuffers(1, &m_animationBuffer);
	m_animations.clear();
}

int Skeleton::AddAnimation(glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame)
{
	if (numFrames > 0 && numBonesPerFrame > 0 && m_numFrames < MAX_FRAMES)
	{
		AnimationInfo info;
		info.offset = 0;

		// Loop through all animations currently bound to this skeleton to find the frame offset
		for (unsigned int i = 0; i < m_numAnimations; i++)
		{
			info.offset = m_animationInfo[i].numFrames * m_animationInfo[i].numBonesPerFrame + m_animationInfo[i].offset;
		}

		info.numFrames = numFrames;
		info.numBonesPerFrame = numBonesPerFrame;

		for (unsigned int f = 0; f < numFrames; f++)
		{
			for (unsigned int b = 0; b < numBonesPerFrame; b++)
				m_animations.push_back(frames[numBonesPerFrame * f + b]);
			++m_numFrames;
		}
		m_animationInfo.push_back(info);
		unsigned int ret = m_numAnimations;
		m_numAnimations++;
		return ret;
	}
	else
	{
		return GFX_INVALID_ANIMATION;
	}
}

int Skeleton::GetInfo(const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset)
{
	if (animationID >= 0 && animationID < m_animationInfo.size())
	{
		out_frameCount = m_animationInfo[animationID].numFrames;
		out_bonesPerFrame = m_animationInfo[animationID].numBonesPerFrame;
		out_animationOffset = m_animationInfo[animationID].offset;
		
		return GFX_SUCCESS;
	}
	else
		return GFX_INVALID_ANIMATION;
}

void Skeleton::BindBuffersData()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_animationBuffer);

	glm::mat4x4* pData = (glm::mat4x4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_FRAMES * sizeof(glm::mat4x4), 
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	memcpy(pData, m_animations.data(), m_animations.size() * sizeof(glm::mat4x4));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}

void Skeleton::BindBuffers()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_animationBuffer);
}
