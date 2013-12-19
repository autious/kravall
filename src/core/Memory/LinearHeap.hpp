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
		   
		   //T* object = new (GetFinalizerObject(finalizer)) T(args...);
		   //Core::Finalizer* finalizer = AllocateWithFinalizer(sizeof(T));
		   T* object = new (m_allocator.Allocate(sizeof(T))) T( args... );
		   Core::Finalizer* destructors = new (m_allocator.Allocate(sizeof(Core::Finalizer))) Core::Finalizer;

		   destructors->m_dataPointer = object;
		   destructors->m_destructorCall = &DestructorCall<T>;
		   destructors->m_finalizerChain = m_finalizerChain;
		   m_finalizerChain = destructors;

		   return object;

			///finalizer->m_destructorCall = &DestructorCall<T>;
			///finalizer->m_finalizerChain = m_finalizerChain;
			///finalizer->m_dataPointer = static_cast<void*>(object);
			///m_finalizerChain = finalizer;
			///return object;
       }

       /*!
         Resets the internal allocator and removes all allocations, object allocations have their destructor called. 
		 */
       void Rewind();

	   /*!
         Allocates a new Plain Old Data structure array with size, destructors will not be called on removal for allocations by this function.
         */
       template<typename T> T* NewPODArray( int size )
       {
           static_assert(std::is_pod<T>::value, "Allocation is not of plain old data type");
           return new (m_allocator.Allocate(sizeof(T) * size)) T;
       }

	   /*!
         Allocates a array of new objects, destructor will be called for every index on remvoval of the allocation. 
         */
       template<typename T, typename... Args> T* NewObjectArray( int size, Args... args)
	   {
			// allocate memory...
			char* objects = reinterpret_cast<char*>(m_allocator.Allocate(sizeof(T) * size));
			Core::Finalizer* destructors = new (m_allocator.Allocate(sizeof(Core::Finalizer) * size)) Core::Finalizer;

			for( int i = 0; i <size; ++i )
			{
				(*static_cast<T*>(reinterpret_cast<void*>(objects+sizeof(T)*i))) = T(args...);
				
				// fix stuff...
				(*static_cast<Core::Finalizer*>(reinterpret_cast<void*>(destructors+sizeof(Core::Finalizer)*i))).m_dataPointer = static_cast<T*>(reinterpret_cast<void*>(objects+sizeof(T)*i));
				(*static_cast<Core::Finalizer*>(reinterpret_cast<void*>(destructors+sizeof(Core::Finalizer)*i))).m_destructorCall = &DestructorCall<T>;
				(*static_cast<Core::Finalizer*>(reinterpret_cast<void*>(destructors+sizeof(Core::Finalizer)*i))).m_finalizerChain = m_finalizerChain;
				m_finalizerChain = static_cast<Core::Finalizer*>(destructors+sizeof(Core::Finalizer)*i);
			}

			return static_cast<T*>(reinterpret_cast<void*>(objects));

			/*
			//Core::Finalizer* finalizer = AllocateWithFinalizer(sizeof(T));
			T* head = new (GetFinalizerObject(finalizer)) T(args...);

			finalizer->m_destructorCall = &DestructorCall<T>;
			finalizer->m_finalizerChain = m_finalizerChain;
			finalizer->m_dataPointer = static_cast<void*>(head);
			m_finalizerChain = finalizer;

			for( int i = 1; i < size; i++ )
			{
			    Core::Finalizer* finalizer = AllocateWithFinalizer(sizeof(T));
			    T* object = new (GetFinalizerObject(finalizer)) T(args...);
			
			    finalizer->m_destructorCall = &DestructorCall<T>;
			    finalizer->m_finalizerChain = m_finalizerChain;
				finalizer->m_dataPointer = static_cast<void*>(object);
			    m_finalizerChain = finalizer;
			}

			return head;
			*/
       }

    private:
       Core::Finalizer* AllocateWithFinalizer(size_t size);

       Core::LinearAllocator m_allocator;
       void* m_rewindPoint;
       Core::Finalizer* m_finalizerChain;
    };
}


#endif
