#ifndef CORE_STACK_HEAP_HPP
#define CORE_STACK_HEAP_HPP



struct Finalizer;

namespace Core
{
    /*!
      Stack based memory manager, all objects that are allocated with the StackHeap are removed once the StackHeap goes out of scope. ObjectsAllocated with AllocateObject have their destructor called on removal.
      */
    class StackHeap
    {
    public:
       /*!
         Constructor for StackHeap.
         \param allocator The linear allocator that is used to allocate memory.
         */
       explicit StackHeap(Core::LinearAllocator& allocator);

       /*!
         Allocates a new Plain Old Data structure, once the StackHeap goes out of scope a destructor will not be called on this allocation.
         */
       template<typename T> T* NewPOD();
       /*!
         Allocates a new object, once the StackHeap goes out of scope a destructor will be called on the new object. 
         */
       template<typename T> T* NewObject();

    private:
       Finalizer* AllocateObjectInternal
       Finalizer* m_finalizerChain;
    };
}


#endif
