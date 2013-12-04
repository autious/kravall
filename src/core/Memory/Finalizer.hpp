#ifndef CORE_MEMORY_FINALIZER_HPP
#define CORE_MEMORY_FINALIZER_HPP

#include <functional>

namespace Core
{
    struct Finalizer
    {
        void (*m_destructorCall)(void* pointer);
        Finalizer* m_finalizerChain;
    };


    template<typename T> 
    void DestructorCall(void* pointer)
    {
        static_cast<T*>(pointer)->~T();
    }
}


#endif
