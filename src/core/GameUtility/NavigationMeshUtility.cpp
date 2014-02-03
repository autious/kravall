#include "NavigationMesh.hpp"
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

#include <limits>

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


    bool NavigationMesh::GetClosestPointInsideMesh( glm::vec3& point, const glm::vec3& origin, int node, int& goalNode )
    {
        if(!CheckPointInsideNode(origin, node))        
		{
			goalNode = -1;
            return false;
		}
        

		Core::NavigationMesh* instance = Core::GetNavigationMesh();
        bool closestPointFound = false;

        glm::vec3 direction = glm::normalize(point - origin);
        float length = glm::length(point - origin);

        while(!closestPointFound)
        {
            float closestDistance = std::numeric_limits<float>::max();
            int cornerId = -1;

            for( int i = 0; i < 4; i++ )
            {
                //if( instance->nodes[node].corners[i].linksTo < -1.5f )
                  //  continue;

                int ii = i * 2;
                int oo = (ii + 2) % 8;
                float* points = instance->nodes[node].points;

                glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
                glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );

                // calc normal
                glm::vec3 planeNormal = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );

                // check collision               
                float dot = glm::dot( planeNormal, direction);
                //The line has to collide with one corner in the node
                if( dot < 0.0f )
                {
                    float distance = glm::dot(lineStart, origin) / dot;
                    if(distance > 0.0f)
                    {
                        if( distance <= closestDistance)
                        {
                            closestDistance = (-dot);
                            cornerId = i;                                   
                        }
                    }
                }                    
            }

            if(closestDistance > length)
            {
                closestPointFound = true;
            }
            else
            {
                //Check if the closest plane is an edge to a neighbour. 
                if(instance->nodes[node].corners[cornerId].linksTo > -1)
                {
                    //If it is a neighbour we restart the algorithm on it.
                    node = instance->nodes[node].corners[cornerId].linksTo;
                }
                else
                {
                    //If it is not a neighbour the closest point from the origin on the mesh is at the intersection point.
                    point = origin + direction * closestDistance;
                    closestPointFound = true;
                }
            }
        }

		goalNode = node;
        return true;
    }


	bool NavigationMesh::CheckPointInsideNode(const glm::vec3& point, int node )
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
			float dot = glm::dot( cross, glm::vec3(point.x, 0.0f, point.z) - lineStart );
			if( dot <= 0 )
				return false;
		}

		// point is inside all lines
		return true;
	}

    bool NavigationMesh::CheckPointInsideNavigationMesh(const glm::vec3& point )
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
