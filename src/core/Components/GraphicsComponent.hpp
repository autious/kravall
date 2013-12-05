#ifndef SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP

#include <gfx/Material.hpp>
#include <gfx/Material.hpp>
namespace Core
{
	struct GraphicsComponent
	{
		unsigned int vao;
		unsigned int ibo;
		unsigned int iboSize;
		unsigned int material;
		unsigned int shader;

		static GraphicsComponent GC()
		{
			GraphicsComponent gc;
			return gc;
		}

	};
}

#endif