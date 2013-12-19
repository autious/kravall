#ifdef RUN_GTEST

#include <gtest/gtest.h>
#include <Memory/LinearHeap.hpp>

// change log; MEMORY_SIZE : expected constant expression @windows -> added const to declaration / John


namespace
{

    const unsigned int MEMORY_SIZE = 1000;

    struct TestObject
    {
        TestObject(void* ptr)
        {
			this->m_ptr = this->m_ptr;
            m_ptr = ptr;
        }

        ~TestObject()
        {
			this->m_ptr = this->m_ptr;
            m_ptr = nullptr;          
        }

        void* m_ptr;
    };

    TestObject* NoOnScopeRemovalTestFunction(Core::LinearHeap linearHeap)
    {
        return linearHeap.NewObject<TestObject>(&linearHeap);
    }

    TEST(LinearHeap, NoOnScopeRemoval)
    {
        unsigned char* Memory[MEMORY_SIZE];

        Core::LinearAllocator alloc(Memory, MEMORY_SIZE);
        Core::LinearHeap linearHeap(alloc);
        TestObject* test_obj = NoOnScopeRemovalTestFunction(linearHeap);

        ASSERT_NE(test_obj->m_ptr, nullptr);            
    }

    TEST(LinearHeap, RewindRemoval)
    {
        unsigned char* Memory[MEMORY_SIZE];

        Core::LinearAllocator alloc(Memory, MEMORY_SIZE);
        Core::LinearHeap linearHeap(alloc);
        TestObject* test_obj = linearHeap.NewObject<TestObject>(&linearHeap);
        char* test_char = linearHeap.NewPOD<char>();

        //Calls destructor for test_obj
        linearHeap.Rewind();

        //Retrieve first address, previously and currently held by test_obj
        test_char = linearHeap.NewPOD<char>();
        
        //Check if destructor was called
        EXPECT_EQ(test_obj->m_ptr, nullptr);

        //Check if we got the first address back after rewind
        ASSERT_EQ(reinterpret_cast<void*>(test_char), reinterpret_cast<void*>(test_obj));
    }

	TEST(LinearHeap, ArrayTest)
    {
		unsigned char* Memory[MEMORY_SIZE];

        Core::LinearAllocator alloc(Memory, MEMORY_SIZE);
        Core::LinearHeap linearHeap(alloc);

		int nrObjects = 10;
		TestObject* list = linearHeap.NewObjectArray<TestObject>( nrObjects, &linearHeap);

		for( int i = 0; i < nrObjects; i++ )
			EXPECT_NE( list[i].m_ptr, nullptr );

		linearHeap.Rewind();		

		for( int i = 0; i < nrObjects; i++ )
		{
			void* pp = list[i].m_ptr;

			EXPECT_EQ( list[i].m_ptr, nullptr );
		}
	}


}
#endif
