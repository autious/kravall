#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <GFXDefines.hpp>
#include "AnimationInfo.hpp"

const unsigned int MAX_FRAMES = 512 * 512;

class Skeleton
{
private:

	struct AnimationInfo
	{
		unsigned int numFrames;
		unsigned int offset;
		unsigned int numBonesPerFrame;
	};
public:
	Skeleton();
	~Skeleton();

	int UpdateInfo(const int& animationID, const unsigned int& offset, const unsigned int& numFrames, const unsigned int& numBonesPerFrame);
	
	/*!
	Adds info about an animation to a skeleton.
	\param offset Offset describing where the animation is stored
	\param numFrames Number of bone matrices in the array
	\param numBonesPerFrame Number of bone matrices per frame
	\return Returns the animation ID if successful, else returns #GFX_INVALID_ANIMATION or #GFX_INVALID_SKELETON
	*/
	int AddAnimationInfo(const unsigned int& offset, const unsigned int& numFrames, const unsigned int& numBonesPerFrame);

	int RemoveAnimation(const int& animationID);
	
	/*!
	Gets the number of frames in a particular animation.
	\param animationID The id of the animation
	\param out_frameCount The number of frames in this animation
	\param out_bonesPerFrame The number of bones per frame
	\return Returns #GFX_SUCCESS if successful, else returns #GFX_INVALID_ANIMATION
	*/
	int GetInfo(const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset);

	/*!
	Gets the number of animations in the skeleton.
	\return Returns the number of animations in the skeleton
	*/
	inline int GetAnimationCount(){ return m_numAnimations; }

	/*!
	Gets the total number of frames in all animations connected to the skeleton.
	\return Returns the number of frames connected to the skeleton
	*/
	inline int GetTotalFrames(){ return m_numFrames; }
	
	//void BindBuffersData();
	//void BindBuffers();

private:
	std::vector<AnimationInfo> m_animationInfo;
	unsigned int m_numAnimations;
	unsigned int m_numFrames;
};

#endif