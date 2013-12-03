#ifndef CM_CONTENT_MANAGER_HPP
#define CM_CONTENT_MANAGER_HPP

#include "ContentManagerInit.h"

#include <future>

namespace CM
{
    DLLSETTING class ContentManager
    {
    public:
        ContentManager();

        template<typename T> void Load(const char* asset)
        {
            
        }

        template<typename T> std::future<T> Load(const char* asset)        
        {
            
        }

    private:

    };
}

#endif
