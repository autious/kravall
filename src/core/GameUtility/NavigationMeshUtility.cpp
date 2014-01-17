#include "NavigationMesh.hpp"
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

namespace Core
{
	void NavigationMesh::DrawDebug()
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();

		for( int i = 0; i < instance->nrNodes; i++ )
		{
			float* points = instance->nodes[i].points;

			for( int p = 0; p < 4; p++ )
			{
				if( instance->nodes[i].corners[p].length < 0 )
					continue;

				int ii = p * 2;
				int oo = (ii + 2) % 8;	

				glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
				GFX::Debug::DrawLine( 
					lineStart, lineEnd, GFXColor( 0.3f, 1.0f, 1.0f, 1.0f ), true );
			}
		}
	}


	bool NavigationMesh::CheckPointInsideNode( glm::vec3 point, int node )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		for( int i = 0; i < 4; i++ )
		{
			if( instance->nodes[node].corners[i].linksTo < -1.5f )
				continue;

			int ii = i * 2;
			int oo = (ii + 2) % 8;
			float* points = instance->nodes[node].points;

			glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
			glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );

			// calc normal
			glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );

			// check if behind plane
			float dot = glm::dot( cross, glm::vec3(point.x, 0.0f, point.y) - lineStart );
			if( dot <= 0 )
				return false;
		}

		// point is inside all lines
		return true;
	}

    bool NavigationMesh::CheckPointInsideNavigationMesh( glm::vec3 point )
    {
        bool found = false;

        for(int i=0; i < nrNodes; ++i)
        {
            found = CheckPointInsideNode(point, i);
            if(found)
                break;
        }

        return found;
    }
}
