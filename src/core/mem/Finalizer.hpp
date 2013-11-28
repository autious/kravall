#ifndef CORE_MEMORY_FINALIZER_HPP
#define CORE_MEMORY_FINALIZER_HPP

#include <functional>

namespace Core
{
    struct Finalizer
    {
        std::function<void(void*)> m_destructorCall;
        Finalizer* m_finalizerChain;
    }


    template<typename T> 
    void DestructorCall(void* pointer)
    {
    static_cast<T*>(pointer)->~T();
    }
}


#endif
