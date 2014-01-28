#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <GFXDefines.hpp>
#include "AnimationInfo.hpp"

class Skeleton
{
private:
	const unsigned int MAX_FRAMES = 512 * 512;

	struct AnimationInfo
	{
		unsigned int numFrames;
		unsigned int offset;
		unsigned int numBonesPerFrame;
	};
public:
	Skeleton();
	~Skeleton();

	/*!
	Adds an animation to a skeleton. The data must contain the same number of bones for all frames
	\param frames Array of bone matrices sorted by frame
	\param numFrames Number of bone matrices in the array
	\param numBonesPerFrame Number of bone matrices per frame
	\return Returns the animation ID if successful, else returns #GFX_INVALID_ANIMATION or #GFX_INVALID_SKELETON
	*/
	int AddAnimation(glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame);
	
	/*!
	Gets the number of frames in a particular animation.
	\param animationID The id of the animation
	\param out_frameCount The number of frames in this animation
	\return Returns #GFX_SUCCESS if successful, else returns #GFX_INVALID_ANIMATION
	*/
	int GetFrameCount( const int& animationID, unsigned int& out_frameCount );

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

	void BindBuffers();

private:
	std::vector<glm::mat4x4> m_animations;
	std::vector<AnimationInfo> m_animationInfo;
	unsigned int m_numAnimations;
	unsigned int m_numFrames;

	GLuint m_animationBuffer;
};

#endif