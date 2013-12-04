#ifdef RUN_GTEST

#include <gtest/gtest.h>
#include <Memory/StackHeap.hpp>

// change log; MEMORY_SIZE : expected constant expression @windows -> added const to declaration / John

namespace
{
    const unsigned int MEMORY_SIZE = 1000;

    struct TestPOD
    {
        float a;
        char s;
        int d;        
    };

    struct TestObject
    {
        TestObject(void* ptr = nullptr)
        {
            m_ptr = ptr;
        }

        ~TestObject()
        {
            m_ptr = nullptr;          
        }

        void* m_ptr;
    };

    TEST(StackAllocator, ClearOnScopeExit)
    {
        unsigned char* Memory[MEMORY_SIZE];
        Core::LinearAllocator alloc(&Memory[0], MEMORY_SIZE);
        
        {
            Core::StackHeap stackAllocator(alloc);            
            TestPOD* testPod = stackAllocator.NewPOD<TestPOD>();
        }

        //Asserts the stack was cleared on out of scope
        ASSERT_EQ(alloc.GetPointer(), &Memory[0]);
    }

    TEST(StackAllocator, AllocateObject)
    {
        unsigned char* Memory[MEMORY_SIZE];
        Core::LinearAllocator alloc(Memory, MEMORY_SIZE);

        TestObject* testObject;
        {
            Core::StackHeap stackAllocator(alloc);
            
            testObject = stackAllocator.NewObject<TestObject>(&Memory[0]);
            //Asserts that the constructor was called
            EXPECT_EQ(testObject->m_ptr, &Memory[0]);
        }
        //Asserts that the destructor was called
        EXPECT_EQ(testObject->m_ptr, nullptr);
    }
}
#endif
