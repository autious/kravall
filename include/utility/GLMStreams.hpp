#ifndef SRC_GLMSTREAMS_H
#define SRC_GLMSTREAMS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <ostream>


inline std::ostream &operator<<( std::ostream &out, const glm::vec2 &vec )
{
    out << "glm::vec2 x:" << vec.x << " y:" << vec.y;
    return out;
}

inline std::ostream &operator<<( std::ostream &out, const glm::vec3 &vec )
{
    out << "glm::vec3 x:" << vec.x << " y:" << vec.y << " z:" << vec.z;
    return out;
}

inline std::ostream &operator<<( std::ostream &out, const glm::vec4 &vec )
{
    out << "glm::vec3 x:" << vec.x << " y:" << vec.y << " z:" << vec.z << " w:" << vec.w;
    return out;
}

inline std::ostream &operator<<( std::ostream &out, const glm::mat4 &mat )
{
    for( int x = 0; x < 4; x++ )
    {
        for( int y = 0; y < 4; y++ )
        {
            out << mat[x][y] << " " ;
        }
            out << std::endl;
    }
    return out;
}

#endif

