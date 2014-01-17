#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <GFXDefines.hpp>
#include "AnimationInfo.hpp"

class Skeleton
{
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
private:
	GLuint m_animationTexture;
	std::vector<AnimationInfo> m_animationInfo;
	unsigned int m_numAnimations;
};

#endif