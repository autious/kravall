#ifndef CORE_LINEAR_ALLOCATOR_HPP
#define CORE_LINEAR_ALLOCATOR_HPP

#include <cstring>

namespace Core
{
    /*!
        LinearAllocator is a custom allocator that allocates a given set of memory. The allocator works with both stack and heap memory as base memory. Only the provided memory will be used for allocation. 
    */
    class LinearAllocator
    {
    public:
        /*!
            Constructor for LinearAllocator. 
            \param memoryPointer A pointer to the memory that is used for allocation. 
            \param size The size in bytes of the memory pointed to by memoryPointer.
        */
        LinearAllocator(void* memoryPointer, size_t size); 

        /*!
            Returns a pointer to allocated memory of the specified size.
            \param size The size in bytes for the allocation.
        */
        void* Allocate(const size_t size);

        /*!
            Resets the internal memory pointer, effectively invalidating all allocated memory.
        */
        void Rewind();

        /*!
            Sets the internal memory pointer to the given pointer. Allocations after the given pointer are no longer valid.
            \param pointer The address to set the internal pointer to.
        */
        void Rewind(void* pointer);

    private:
        LinearAllocator();

        unsigned char* m_memoryPointer;
        unsigned char* m_startPointer;
        unsigned char* m_endPointer;
    };
}

#endif
