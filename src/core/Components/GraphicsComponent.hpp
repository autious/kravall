#ifndef SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP

#include <gfx/Material.hpp>
#include <gfx/BitmaskDefinitions.hpp>
namespace Core
{
	struct GraphicsComponent
	{
		unsigned int vao;
		unsigned int ibo;
		unsigned int iboSize;
		GFX::Material* material;
		unsigned int shader;

		GFX::GFXBitmask bitmask;

		static GraphicsComponent GC()
		{
			GraphicsComponent gc;
			return gc;
		}
	};
}

#endif