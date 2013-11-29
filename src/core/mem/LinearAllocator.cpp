#include "LinearAllocator.hpp"

namespace Core
{
    LinearAllocator::LinearAllocator(void* memoryPointer, size_t size)
    {
        m_startPointer = m_memoryPointer = static_cast<unsigned char*>(memoryPointer);
        m_endPointer = m_memoryPointer + size;
    }

    void* LinearAllocator::Allocate(const size_t size)    
    {
        unsigned char* pointer = m_memoryPointer + size > m_endPointer ? nullptr : m_memoryPointer;
        m_memoryPointer += size;
        return pointer;
    }

    void LinearAllocator::Rewind()
    {
        m_memoryPointer = m_startPointer;
    }

    void LinearAllocator::Rewind(void* pointer)
    {
        m_memoryPointer = static_cast<unsigned char*>(pointer);
    }

    void* LinearAllocator::GetPointer() const
    {
        return m_memoryPointer;
    }
}
