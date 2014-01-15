#ifndef SRC_GFX_INSTANCE_DATA_HPP
#define SRC_GFX_INSTANCE_DATA_HPP

#include <glm/glm.hpp>


namespace GFX
{
	struct StaticMeshInstanceData
	{
		glm::mat4 modelMatrix;
	};

	struct AnimatedMeshInstanceData
	{
		glm::mat4 modelMatrix;
		int animationFrame;
		int animationFrameOffset;
	};


}

#endif