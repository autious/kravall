#ifndef CORE_TEMP_HEAP_HPP
#define CORE_TEMP_HEAP_HPP

#include <Memory/Finalizer.hpp>
#include <Memory/LinearAllocator.hpp>

#include <cstring>
#include <type_traits>

namespace Core
{
    /*!
     Memory manager for user defined life time allocations, all objects that are allocated with the LinearHeap are invalidated on call to Rewind. Objects allocated with AllocateObject have their destructor called on removal.
     */
    class LinearHeap
    {
    public:
       /*!
         Constructor for LinearHeap.
         \param allocator The linear allocator that is used to allocate memory.
         */
       explicit LinearHeap(Core::LinearAllocator& allocator);
       
       /*!
         Allocates a new Plain Old Data structure, destructors will not be called on removal for allocations by this function.
         */
       template<typename T> T* NewPOD()
       {
           static_assert(std::is_pod<T>::value, "Allocation is not of plain old data type");
           return new (m_allocator.Allocate(sizeof(T))) T;
       }

       /*!
         Allocates a new object, destructor will be called on remvoval of the allocation. 
         */
       template<typename T, typename... Args> T* NewObject(Args... args)
       {
           Core::Finalizer* finalizer = AllocateWithFinalizer(sizeof(T));
           T* object = new (GetFinalizerObject(finalizer)) T(args...);

           finalizer->m_destructorCall = &DestructorCall<T>;
           finalizer->m_finalizerChain = m_finalizerChain;
           m_finalizerChain = finalizer;
           return object;
       }

       /*!
         Resets the internal allocator and removes all allocations, object allocations have their destructor called. 
         */
       void Rewind();

    private:
       Core::Finalizer* AllocateWithFinalizer(size_t size);
       inline void* GetFinalizerObject(Core::Finalizer* finalizer)
       {
           return reinterpret_cast<unsigned char*>(finalizer) + sizeof(Core::Finalizer);
       }

       Core::LinearAllocator m_allocator;
       void* m_rewindPoint;
       Core::Finalizer* m_finalizerChain;
    };
}


#endif
