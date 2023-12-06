#include <iostream>
#include <simpletest.h>
#include "BumpUp.hpp"
using namespace std;



char const *groups[] = {
    "BumpUp",
    
};

DEFINE_TEST_G(BasicAllocTest, BumpUp)
{
    BumpUp<20 * sizeof(int)> bumper;

    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate");

    int *y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate");

    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate");
}

DEFINE_TEST_G(BasicDeallocTest, BumpUp)
{
    BumpUp<1024> bumper;
    int *ptr1 = bumper.alloc<int>();
    double *ptr2 = bumper.alloc<double>(5);
    char *ptr3 = bumper.alloc<char>(10);
    // Check allocations are successful
    TEST_MESSAGE(ptr1 != nullptr, "Failed to allocate");
    TEST_MESSAGE(ptr2 != nullptr, "Failed to allocate");
    TEST_MESSAGE(ptr3 != nullptr, "Failed to allocate");

    int numAllocations = 3;
    // Deallocate allocations
    for (int i = 0; i < numAllocations; ++i)
    {
        bumper.dealloc();
    }

    // Check if alloc is reset
    // If next is in the correct position then deallocations successful
    TEST_MESSAGE(bumper.getPtrPosition() == 0 || bumper.getPtrPosition() == 1024, "Bumper failed to deallocate and reset");
}

DEFINE_TEST_G(TestAlignment, BumpUp)
{
    // Testing alignment
    BumpUp<1024> bumper;

    // int has 4 byte alignment
    int *intPtr = bumper.alloc<int>();
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0, "Failed integer alignment test");

    // double has 8 byte alignment
    double *doublePtr = bumper.alloc<double>();
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0, "Failed double alignment test");
}


DEFINE_TEST_G(TestVaryingTypes, BumpUp)
{
    BumpUp<4096> allocator;                // larger size to test
    int *intPtr = allocator.alloc<int>(100); // Allocate 100 ints, should be 400 byte
    TEST_MESSAGE(intPtr != nullptr, "Failed to allocate 100 ints");
    double *dblPtr = allocator.alloc<double>(200); // Allocate 200 doubles, should be 1600 bytes
    TEST_MESSAGE(dblPtr != nullptr, "Failed to allocate 200 doubles");

    char *charPtr = allocator.alloc<char>(96);
    TEST_MESSAGE(charPtr != nullptr, "Failed to allocate 96 char");

    double *dblPtr2 = allocator.alloc<double>(250); // Allocate 250 double, should be 2000 bytes
    TEST_MESSAGE(dblPtr2 != nullptr, "Failed to allocate 250 doubles");
}

DEFINE_TEST_G(BigAllocationsTest, BumpUp)
{
    BumpUp<1024 * 1024> allocator; // Large size to accommodate big allocations

    int numAllocations = 10;
    int sizePerAllocation = 1024 * 10; // Big allocations (10 KB each)
    char* allocations[numAllocations];

    for (int i = 0; i < numAllocations; ++i) {
        allocations[i] = allocator.alloc<char>(sizePerAllocation);
        TEST_MESSAGE(allocations[i] != nullptr, "Failed to allocate big chunk");
    }

    
}

DEFINE_TEST_G(AlignmentAndPaddingTest, BumpUp){

    
    BumpUp<1024> allocator;

    // Allocate a char 
    char* charPtr = allocator.alloc<char>();    

    int* intPtr = allocator.alloc<int>();    

    // Calculate padding between char and int allocations, reinterpret into char as has sizeof 1 byte so easily observable
    ptrdiff_t padding = reinterpret_cast<char*>(intPtr) - (charPtr + sizeof(char));
    

    // Assert that int is correctly aligned
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0, "Failed to align int");
    
    // Assert that padding is within expected limits (0 to 3)
    TEST_MESSAGE(padding >= 0 && padding < alignof(int), "Padding incorrectly calculated");

    // Allocate a double 8 byte alignment
    double* doublePtr = allocator.alloc<double>();
    
    // Calculate padding between int and double allocations, reinterpret into char as has sizeof 1 byte so easy to calculate padding
    padding = reinterpret_cast<char*>(doublePtr) - (reinterpret_cast<char*>(intPtr) + sizeof(int));
    

    // Assert that double is correctly aligned
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0, "Failed to align double");

    // make sure that padding is within expected limits (0 to 7)
    TEST_MESSAGE(padding >= 0 && padding < alignof(double), "Padding incorrectly calculated");

    
}

int main()
{   bool pass = true;

    for (auto group : groups){    
    
    pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);        
        
        
    }   
    
    return pass ? 0 : 1;    
}

// clang++ -std=c++17 -O2 -I../simpletest_test/simpletest/ task_2_tests.cpp ../simpletest_test/simpletest/simpletest.cpp