#ifndef CORE_STACK_HEAP_HPP
#define CORE_STACK_HEAP_HPP

#include <mem/Finalizer.hpp>
#include <mem/LinearAllocator.hpp>

#include <cstring>
#include <type_traits>

namespace Core
{
    /*!
      Stack based memory manager, all objects that are allocated with the StackHeap are removed once the StackHeap goes out of scope. Objects allocated with AllocateObject have their destructor called on removal.
     */
    class StackHeap
    {
    public:
       /*!
         Constructor for StackHeap.
         \param allocator The linear allocator that is used to allocate memory.
         */
       explicit StackHeap(Core::LinearAllocator& allocator);
       ~StackHeap();
       
       /*!
         Allocates a new Plain Old Data structure, once the StackHeap goes out of scope a destructor will not be called on this allocation.
         */
       template<typename T> T* NewPOD()
       {
           static_assert(std::is_pod<T>::value, "Allocation is not of plain old data type");
           return new (m_allocator.Allocate(sizeof(T))) T;
       }

       /*!
         Allocates a new object, once the StackHeap goes out of scope a destructor will be called on the new object. 
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
