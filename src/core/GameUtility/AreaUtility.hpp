namespace Core
{
    namespace AreaUtility
    {
        inline bool CheckPointInsideNode( glm::vec3 point, float * points, glm::vec3 pointsOrigo )
        {
            for( int i = 0; i < 4; i++ )
            {
                int ii = i * 2;
                int oo = (ii + 2) % 8;

                glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] ) + pointsOrigo;
                glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] ) + pointsOrigo;

                // calc normal
                glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );

                // check if behind plane
                float dot = glm::dot( cross, point - lineStart );
                if( dot <= 0 )
                    return false;
            }

            // point is inside all lines
            return true;
        }
    }
}
