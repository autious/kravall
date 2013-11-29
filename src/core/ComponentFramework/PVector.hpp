#ifndef SRC_CORE_COMPONENTFRAMEWORK_PVECTOR_H
#define SRC_CORE_COMPONENTFRAMEWORK_PVECTOR_H

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cassert>

namespace Core
{
    class PVector
    {
    private:
        void *m_data = nullptr;
        size_t m_size;
        size_t m_count;
        size_t m_growStep;
        size_t m_typesize;
        std::vector<int> deleted;
    public:

        /*!
            Creates an instance of a packed vector with the initials memory size 
            of initialSize * sizeof(Component).
            
            /param growStep size step growth for each time the array isn't large enough.
        */
        PVector( size_t initialSize, size_t growStep, size_t typesize );

        ~PVector( );

        /*!
            Adds a given data into the array.
            If there are "holes" in the array, 
            this function will primarily reuse 
            that memory.

            Whenever this function is called, all pointers
            to data in this structure are invalidated.

            \param source data to copy into index 
        */
        int Alloc( );

        /*!
            Releses a component from the array, making it available 
            for reallocation.
        */
        void Release( int id );

        /*!
            Retrieves a temporary pointer to data in
            this structure.
        */
        template<class Component>
        Component* Get( int id )
        {
            assert( sizeof(Component) == m_typesize );
            return (Component*)Get(id);
        }

        void* Get( int id );

        /*!
            Returns how many active components there are.
        */
        size_t GetCount();
    
        /*!
            Returns how many components slots there are allocated in memory.
        */
        size_t GetAllocation();
    };
}

#endif
