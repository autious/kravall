#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include <GL/glew.h>
#include <vector>
#include "Skeleton.hpp"

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();
private:
	std::vector<Skeleton> m_skeletons;
};

#endif