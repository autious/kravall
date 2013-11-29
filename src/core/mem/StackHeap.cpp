#include "StackHeap.hpp"

namespace Core
{
    StackHeap::StackHeap(Core::LinearAllocator& allocator):
        m_allocator(allocator),
        m_rewindPoint(allocator.GetPointer()),
        m_finalizerChain(nullptr)
        {}

    StackHeap::~StackHeap()
    {
        for(Finalizer* f = m_finalizerChain; f; f = f->m_finalizerChain)
        {
           f->m_destructorCall(GetFinalizerObject(f)); 
        }
        m_allocator.Rewind(m_rewindPoint);
    }

    Core::Finalizer* StackHeap::AllocateWithFinalizer(size_t size)
    {
        return static_cast<Core::Finalizer*>(m_allocator.Allocate(size + sizeof(Core::Finalizer)));
    }
}
