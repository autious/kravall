#include <cstdint>
#include <cstdlib>
#include <vector>

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
        PVector( size_t initialSize, size_t growStep, size_t typesize )
        {
            m_data = malloc( initialSize * typesize );
            m_size = initialSize;
            m_count = 0;
            m_growStep = growStep;
            m_typesize = typesize;
        }

        ~PVector( )
        {
            free( m_data );
        }

        /*!
            Adds a given data into the array.
            If there are "holes" in the array, 
            this function will primarily reuse 
            that memory.

            Whenever this function is called, all pointers
            to data in this structure are invalidated.

            \param source data to copy into index 
        */
        int Alloc( )
        {
            int id = -1;
            if( m_count >= m_size )
            {
                m_size += m_growStep;
                m_data = realloc( m_data, m_size * m_typesize );

                assert( m_data != NULL );
            } 

            if( deleted.size() > 0 )
            {
                id = deleted.back();
                deleted.pop_back();
            }
            else
            {
                id = m_count;
            }

            m_count++;

            return id;
        }

        /*!
            Releses a component from the array, making it available 
            for reallocation.
        */
        void Release( int id )
        {
            deleted.push_back( id );
            m_count--;
        }

        /*!
            Retrieves a temporary pointer to data in
            this structure.
        */
        template<class Component>
        Component* Get( int id )
        {
            assert( sizeof(Component) == m_typesize );
            assert( id > 0 && id < m_count );
            return &((Component*)m_data)[id];
        }

        /*!
            Returns how many active components there are.
        */
        size_t GetCount()
        {
            return m_count;
        }
    
        /*!
            Returns how many components slots there are allocated in memory.
        */
        size_t GetAllocation()
        {
            return m_size;
        }
    };
}
