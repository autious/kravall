#ifndef SRC_GFX_INSTANCE_DATA_HPP
#define SRC_GFX_INSTANCE_DATA_HPP

#include <glm/glm.hpp>


namespace GFX
{
	struct InstanceData
	{
		glm::mat4 modelMatrix;
		unsigned int animationFrame;
		unsigned int animationFrameOffset;
		unsigned int pad1;
		unsigned int pad2;
	};

	//struct StaticMeshInstanceData
	//{
	//	glm::mat4 modelMatrix;
	//};
	//
	//struct AnimatedMeshInstanceData
	//{
	//	glm::mat4 modelMatrix;
	//	int animationFrame;
	//	int animationFrameOffset;
	//};


}

#endif