#ifndef SRC_GFX_SHADOWDATA_HPP
#define SRC_GFX_SHADOWDATA_HPP

#include <glm/glm.hpp>


struct ShadowData
{
	glm::mat4x4 lightMatrix1;
	glm::mat4x4 lightMatrix2;
	glm::mat4x4 lightMatrix3;
	glm::mat4x4 lightMatrix4;
	glm::vec4 atlasCoords;
};

static const unsigned int MAXIMUM_SHADOWCASTERS = 16;

class ShadowDataContainer
{
public:
	static ShadowData data[MAXIMUM_SHADOWCASTERS];
	static unsigned int numDirLights;
	static unsigned int numSpotLights;
	static unsigned int numPointLights;
};


#endif