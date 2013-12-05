#ifndef CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_MODEL_DATA_HPP
#define CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_MODEL_DATA_HPP

#include <GL/glew.h>

namespace Core
{
    struct ModelData
    {
        GLuint IBO;
        GLuint VAO;
        GLint vSize;
        GLint iSize;
    };
}

#endif
