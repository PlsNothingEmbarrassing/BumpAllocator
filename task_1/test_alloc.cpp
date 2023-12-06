#include <iostream>
#include "bump_alloc.hpp"
#include <cassert>
// Basic tests to functionality
void testBasicAllocation() {
    BumpAllocator<1024> allocator; // Allocating 1024 bytes of memory

    int* intPtr = allocator.alloc<int>();
    assert(intPtr != nullptr);

    double* doublePtr = allocator.alloc<double>();
    assert(doublePtr != nullptr);

    std::cout << "Test Basic Allocation: Passed" << std::endl;
}


void testMemoryExhaustion() {
    BumpAllocator<64> allocator; // Small size to test exhaustion

    int* ptr1 = allocator.alloc<int>(10); // Allocating space for 10 ints
    assert(ptr1 != nullptr);

    int* ptr2 = allocator.alloc<int>(100); // Should fail as there isnt space
    assert(ptr2 == nullptr);

    std::cout << "Test Memory Exhaustion: Passed" << std::endl;
}

void testAlignment() {
    BumpAllocator<1024> allocator;

    // int requires 4 byte alignment
    int* intPtr = allocator.alloc<int>();
    assert(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0);

    // double requires 8 byte alignment
    double* doublePtr = allocator.alloc<double>();
    assert(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0);

    std::cout << "Test Alignment: Passed" << std::endl;
}

void testPaddingAndAlignment() {
    BumpAllocator<1024> allocator;

    // Allocate a char 
    char* charPtr = allocator.alloc<char>();    

    int* intPtr = allocator.alloc<int>();    

    // Calculate padding between char and int allocations, reinterpret into char as has sizeof 1 byte so easily observable
    std::ptrdiff_t padding = reinterpret_cast<char*>(intPtr) - (charPtr + sizeof(char));
    std::cout << "Padding between char and int: " << padding << " bytes" << std::endl;

    // Assert that int is correctly aligned
    assert(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0);
    
    // Assert that padding is within expected limits (0 to 3)
    assert(padding >= 0 && padding < alignof(int));

    // Allocate a double 8 byte alignment
    double* doublePtr = allocator.alloc<double>();    

    // Calculate padding between int and double allocations, reinterpret into char as has sizeof 1 byte so easy to calculate padding
    padding = reinterpret_cast<char*>(doublePtr) - (reinterpret_cast<char*>(intPtr) + sizeof(int));
    std::cout << "Padding between int and double: " << padding << " bytes" << std::endl;

    // Assert that double is correctly aligned
    assert(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0);

    // make sure that padding is within expected limits (0 to 7)
    assert(padding >= 0 && padding < alignof(double));

    std::cout << "Test Padding and Alignment: Passed" << std::endl;
}

void testDeallocation(){
    BumpAllocator<1024> allocator;

    int* ptr1 = allocator.alloc<int>();
    double* ptr2 = allocator.alloc<double>(5);
    char* ptr3 = allocator.alloc<char>(10);
    // Check allocations are successful
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);
    assert(ptr3 != nullptr);

    int numAllocations = 3;
    // Deallocate allocations
    for (int i = 0; i < numAllocations; ++i){
        allocator.dealloc();
    }

    // Check if alloc is reset (0 for bump up and 1024 for bump down)
    assert(allocator.getPtrPosition() == 0 || allocator.getPtrPosition() == 1024);
    // If next is in the correct position then deallocations successful

    int* ptr4 = allocator.alloc<int>(10);
    assert(ptr4 != nullptr && (allocator.getPtrPosition() == 40 || allocator.getPtrPosition() == 984));
    // Test that ptr has moved to correct position after deallocation and allocations can be done
    std::cout << "Test Deallocation: Passed" << std::endl;
}

int main(){
    testBasicAllocation();
    testMemoryExhaustion();
    testAlignment();
    testDeallocation();
    testPaddingAndAlignment();
    return 0;
}