#include "PVector.hpp"

#include <iostream>

Core::PVector::PVector( size_t initialSize, size_t growStep, size_t typesize )
{
    m_data = malloc( initialSize * typesize );
    m_size = initialSize;
    m_count = 0;
    m_growStep = growStep;
    m_typesize = typesize;
}

Core::PVector::~PVector( )
{
    free( m_data );
}

int Core::PVector::Alloc( )
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

void Core::PVector::Release( int id )
{
    deleted.push_back( id );
    m_count--;
}

void* Core::PVector::Get( int id )
{
    assert( id >= 0 && id < m_count );
    return &(((unsigned char*)m_data)[id*m_typesize]);
}

void Core::PVector::Set( int id, const void *component )
{
    memcpy( Get(id), component, m_typesize );
}

size_t Core::PVector::GetCount()
{
    return m_count;
}

size_t Core::PVector::GetAllocation()
{
    return m_size;
}
