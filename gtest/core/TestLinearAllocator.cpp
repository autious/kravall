#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <mem/LinearAllocator.hpp>

namespace 
{
    TEST(LinearAllocatorTest, StackMemoryAllocation)
    {
        const int MEMORY_SIZE = 4000;
        unsigned char memory[MEMORY_SIZE];

        unsigned char* mem_addr =  &memory[0];
        Core::LinearAllocator linearAlloc(&memory[0], MEMORY_SIZE);       
        unsigned char* ret_addr = static_cast<unsigned char*>( linearAlloc.Allocate(8) );
        ASSERT_EQ(mem_addr, ret_addr);
    }

    TEST(LinearAllocatorTest, HeapMemoryAllocation)
    {
        const int MEMORY_SIZE = 4000;
        unsigned char* memory = new unsigned char[MEMORY_SIZE];

        void* mem_addr = &memory[0];
        Core::LinearAllocator linearAlloc(&memory[0], MEMORY_SIZE);       
        void* ret_addr = linearAlloc.Allocate(8);
        ASSERT_EQ(mem_addr, ret_addr);
        delete[] memory;
    }

    TEST(LinearAllocatorTest, MultiAllocation)
    {
        const int MEMORY_SIZE = 4000;
        unsigned char memory[MEMORY_SIZE];

        unsigned char* mem_addr = &memory[0];
        Core::LinearAllocator linearAlloc(&memory[0], MEMORY_SIZE);       

        unsigned char* prev_addr = mem_addr;
        for(int i = 1; i < 30; ++i)
        {
            unsigned char* ret_addr = static_cast<unsigned char*>( linearAlloc.Allocate(i) );
            ASSERT_EQ(prev_addr, ret_addr);
            prev_addr+=i;
        }    
    }

    TEST(LinearAllocatorTest, BoundaryAllocation)
    {
        const int MEMORY_SIZE = 4000;
        unsigned char memory[MEMORY_SIZE];

        Core::LinearAllocator linearAlloc(&memory[0], MEMORY_SIZE);       

        void* mem_addr = linearAlloc.Allocate(MEMORY_SIZE);
        ASSERT_NE(mem_addr, nullptr);
    }

    TEST(LinearAllocatorTest, MoreThanMaxAllocation)
    {   
        const int MEMORY_SIZE = 4000;
        unsigned char memory[MEMORY_SIZE];

        Core::LinearAllocator linearAlloc(&memory[0], MEMORY_SIZE);       

        void* mem_addr = linearAlloc.Allocate(MEMORY_SIZE+1);
        ASSERT_EQ(mem_addr, nullptr);
    }
}
#endif
