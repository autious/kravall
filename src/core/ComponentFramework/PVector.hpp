#include <cstdint>
#include <cstdlib>
#include <vector>

namespace Core
{
    template<typename Component>
    class PVector
    {
    private:
        Component* m_data = nullptr;
        size_t m_size;
        size_t m_count;
        size_t m_growStep;
        std::vector<int> deleted;
    public:

        /*!
            Creates an instance of a packed vector with the initials memory size 
            of initialSize * sizeof(Component).
            
            /param growStep size step growth for each time the array isn't large enough.
        */
        PVector( size_t initialSize, size_t growStep )
        {
            static_assert(std::is_pod<Component>::value, "******Components must be Pure Data Objects*******");

            m_data = (Component*)malloc( initialSize * sizeof( Component) );
            m_size = initialSize;
            m_count = 0;
            m_growStep = growStep;
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
                m_data = (Component*)relloc( m_data, m_size * sizeof( Component ) );

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
        Component* Get( int id )
        {
            assert( id > 0 && id < m_count );
            return &m_data[id*sizeof(Component)]; 
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
