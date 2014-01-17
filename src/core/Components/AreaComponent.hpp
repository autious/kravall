#ifndef SRC_CORE_COMPONENTS_AREACOMPONENT_H
#define SRC_CORE_COMPONENTS_AREACOMPONENT_H

#include <glm/glm.hpp>

namespace Core
{
	/*!
		Component specifying a 2D area with WorldPositionComponent as origo.
        Spans over x and z axis.
        Consists of 4 vertices (a quad)
	*/
	struct AreaComponent
	{
        static const int DATA_COUNT = 2*4;
        float vertices[DATA_COUNT];

        inline static const char* GetName()
        {
            return "AreaComponent";
        }

        inline static glm::vec3 GetVec3( const AreaComponent& ac, int vertice )
        {
            assert( vertice < 4 && vertice >= 0 );
            return glm::vec3( ac.vertices[vertice*2], 0, ac.vertices[vertice*2+1] );
        }
	};
}
#endif
