#include "LinearHeap.hpp"

#include <vector>

namespace Core
{
    LinearHeap::LinearHeap(Core::LinearAllocator& allocator):
        m_allocator(allocator),
        m_rewindPoint(allocator.GetPointer()),
        m_finalizerChain(nullptr)
        {}

    Core::Finalizer* LinearHeap::AllocateWithFinalizer(size_t size)
    {
        return static_cast<Core::Finalizer*>(m_allocator.Allocate(size + sizeof(Core::Finalizer)));
    }


    void LinearHeap::Rewind()
    {       
        for(Finalizer* f = m_finalizerChain; f; f = f->m_finalizerChain)
        {
			f->m_destructorCall( f->m_dataPointer );
        }
		m_finalizerChain = nullptr;
        m_allocator.Rewind(m_rewindPoint);
    }
}
