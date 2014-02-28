#ifndef SRC_GFX_MATERIAL_DATA_HPP
#define SRC_GFX_MATERIAL_DATA_HPP

#include <vector>
#include <GL/glew.h>

namespace GFX
{

	struct Material
	{
		unsigned int id;
		unsigned int shaderProgramID;
		std::vector<unsigned long long int> textures;
	};
}

#endif