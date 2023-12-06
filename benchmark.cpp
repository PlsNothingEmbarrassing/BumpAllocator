#include "BumpUp.hpp"
#include "BumpDown.hpp"
#include "benchmark.h"
#include <simpletest.h>
#include <chrono>
#include <functional>

#include <tuple>
#include <thread>
using namespace std;



char const *groups[] = {
    "BumpUp",
    "BumpDown",
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
    auto *ptr1 = bumper.alloc<int>();
    auto *ptr2 = bumper.alloc<double>(5);
    auto *ptr3 = bumper.alloc<char>(10);
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

    // Assuming int requires 4 byte alignment
    int *intPtr = bumper.alloc<int>();
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0, "Failed integer alignment test");

    // Assuming double requires 8 byte alignment
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

    constexpr int numAllocations = 10;
    constexpr int sizePerAllocation = 1024 * 10; // Big allocations (10 KB each)
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

DEFINE_TEST_G(BasicAllocTest, BumpDown)
{
    BumpDown<20 * sizeof(int)> bumper;

    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate");

    int *y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate");

    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate");
}

DEFINE_TEST_G(BasicDeallocTest, BumpDown)
{
    BumpDown<1024> bumper;
    auto *ptr1 = bumper.alloc<int>();
    auto *ptr2 = bumper.alloc<double>(5);
    auto *ptr3 = bumper.alloc<char>(10);
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

DEFINE_TEST_G(TestAlignment, BumpDown)
{
    // Testing alignment
    BumpDown<1024> bumper;

    // Assuming int requires 4 byte alignment
    int *intPtr = bumper.alloc<int>();
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0, "Failed integer alignment test");

    // Assuming double requires 8 byte alignment
    double *doublePtr = bumper.alloc<double>();
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0, "Failed double alignment test");
}


DEFINE_TEST_G(TestVaryingTypes, BumpDown)
{
    BumpDown<4096> allocator;                // larger size to test
    int *intPtr = allocator.alloc<int>(100); // Allocate 100 ints, should be 400 byte
    TEST_MESSAGE(intPtr != nullptr, "Failed to allocate 100 ints");
    double *dblPtr = allocator.alloc<double>(200); // Allocate 200 doubles, should be 1600 bytes
    TEST_MESSAGE(dblPtr != nullptr, "Failed to allocate 200 doubles");

    char *charPtr = allocator.alloc<char>(96);
    TEST_MESSAGE(charPtr != nullptr, "Failed to allocate 96 char");

    double *dblPtr2 = allocator.alloc<double>(250); // Allocate 250 double, should be 2000 bytes
    TEST_MESSAGE(dblPtr2 != nullptr, "Failed to allocate 250 doubles");

    
}

DEFINE_TEST_G(BigAllocationsTest, BumpDown)
{
    BumpDown<1024 * 1024> allocator; // large size for big allocations

    constexpr int numAllocations = 10;
    constexpr int sizePerAllocation = 1024 * 10; // big allocations 10 kb each
    char* allocations[numAllocations];

    for (int i = 0; i < numAllocations; ++i) {
        allocations[i] = allocator.alloc<char>(sizePerAllocation);
        TEST_MESSAGE(allocations[i] != nullptr, "Failed to allocate big chunk");
    }

    
}

DEFINE_TEST_G(AlignmentAndPaddingTest, BumpDown){
    // Order of allocations is reversed for the bumpdown
    BumpDown<1024> allocator;

    // Allocate a double
    double* doublePtr = allocator.alloc<double>();
    // Allocate an int
    int* intPtr = allocator.alloc<int>();    

    // Calculate padding between double and int allocations, reinterpret into char as has sizeof 1 byte so easily observable
    ptrdiff_t padding = reinterpret_cast<char*>(doublePtr) - (reinterpret_cast<char*>(intPtr) + sizeof(int));
    

    // Assert that double is correctly aligned
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0, "Failed to align double");;
    
    // Assert that padding is within expected limits (0 to 7)
    TEST_MESSAGE(padding >= 0 && padding < alignof(double), "Padding incorrectly calculated for double");

    // Allocate a char
    char* charPtr = allocator.alloc<char>();
    

    // Calculate padding between int and double allocations, reinterpret into char as has sizeof 1 byte so easy to calculate padding
    padding = reinterpret_cast<char*>(intPtr) - (charPtr + sizeof(char));
    

    // Assert that int is correctly aligned
    TEST_MESSAGE(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0, "Failed to align int");

    // make sure that padding is within expected limits (0 to 3)
    TEST_MESSAGE(padding >= 0 && padding < alignof(int), "Padding incorrectly calculated for int");

    
}


int runTests(char const* group)
{
    bool pass = true;
    
    pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    
    return pass ? 0 : 1;
}

int main()
{
    for (auto group : groups){
        // Timing mutiple runs for each group and then take average
        double totalTime = 0.0;
        int numRuns = 5;
        cout << "Running tests for group "<< group << endl;
        for(int i =0; i < numRuns; ++i){
            // report time and the benchmark function it calls is a variadric template so I can pass as many args as I want 
           auto execution_time = report_time("runTests", runTests, group);
           totalTime+=execution_time;
           cout << "Run " << (i+1) << ": " << execution_time << " nanoseconds" << endl;
        }
        // Output average execution time for each allocator over number of runs
        double averageTime = totalTime / numRuns;
        cout << "Average time for " << group << ": " << averageTime << endl;
        
    }   
    
    

    return 0;
}

// clang++ -std=c++17 -O2 -I./simpletest_test/simpletest/ benchmark.cpp ./simpletest_test/simpletest/simpletest.cpp
// clang++ -std=c++17 -O3 -I./simpletest_test/simpletest/ benchmark.cpp ./simpletest_test/simpletest/simpletest.cpp
