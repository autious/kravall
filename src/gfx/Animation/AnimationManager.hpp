#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include <GL/glew.h>
#include <map>
#include "Skeleton.hpp"
#include <GFXDefines.hpp>

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();
	
	/*!
	Creates a skeleton on the GPU
	\param out_skeletonID Reference returning the id created for the skeleton
	\return Returns #GFX_FAIL if unable to create skeleton, else returns #GFX_SUCCESS
	*/
	int CreateSkeleton(unsigned int& out_skeletonID);

	/*!
	Deletes a skeleton, removing its animations
	\param skeletonID The id of the skeleton to delete
	\return Returns #GFX_FAIL if unable to remove skeleton, else returns #GFX_SUCCESS
	*/
	int DeleteSkeleton(const unsigned int& skeletonID);
	
	/*!
	Adds an animation to a skeleton. The data must contain the same number of bones for all frames
	\param skeletonID Reference id of the skeleton to bind the animation to
	\param frames Array of bone matrices sorted by frame
	\param numFrames Number of bone matrices in the array
	\param numBonesPerFrame Number of bone matrices per frame
	\return Returns the animation ID if successful, else returns #GFX_INVALID_ANIMATION or #GFX_INVALID_SKELETON
	*/
	int AddAnimationToSkeleton(const unsigned int& skeletonID, glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame);

	int GetFrameCount(const unsigned int& skeletonID, const unsigned int& animationID, unsigned int& out_frameCount);

private:
	std::map<unsigned int, Skeleton*> m_skeletons;
	unsigned int m_idCounter;
};

#endif