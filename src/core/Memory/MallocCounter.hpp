#ifndef CORE_MEM_MALLOC_COUNTER_HPP
#define CORE_MEM_MALLOC_COUNTER_HPP

#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <utility>
#include <memory>

namespace Core
{
    template<typename T>
    class MallocCounter
    {
    public:
        MallocCounter()
        {   
            m_maxAllocateSize = 1000 * sizeof(T);
            m_usedMemory = 0;
        }

        ~MallocCounter()
        {

        }

        void* address(T& t) const
        {
            return &t;
        }

        void* address(const T& t) const
        {
            return &t;
        }

        void* allocate(size_t n)
        {
            m_usedMemory += sizeof(T) * n;
            return std::malloc( sizeof(T) * n);
        }

        void deallocate(T* p, size_t n)
        {
            m_usedMemory -= sizeof(T) * n;
            std::free(p);
        }

        size_t max_size() const
        {
            return m_maxAllocateSize;
        }

        template<typename U, typename... Args>
        void construct(U* p, Args&&... args)
        {
            ::new ((void*)p) U(std::forward<Args>(args)...);
        }

        template<typename U>
        void destroy(U* p)
        {
            p->~U();
        }


        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T* const_pointer;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        struct rebind
        {
            typedef std::allocator<T> other;
        };



    private:
        int m_maxAllocateSize;
        int m_usedMemory;

    };
}


#endif
