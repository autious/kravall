#ifndef CORE_STACK_HEAP_HPP
#define CORE_STACK_HEAP_HPP



struct Finalizer;

namespace Core
{
    class StackHeap
    {
    public:
       explicit  StackHeap();

    private:
       Finalizer* m_finalizerChain;
    };
}


#endif
