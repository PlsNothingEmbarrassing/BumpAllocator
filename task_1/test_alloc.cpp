#include <iostream>
#include "bump_alloc.hpp"
#include <cassert>
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

    // Assuming int requires 4 byte alignment
    int* intPtr = allocator.alloc<int>();
    assert(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0);

    // Assuming double requires 8 byte alignment
    double* doublePtr = allocator.alloc<double>();
    assert(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0);

    std::cout << "Test Alignment: Passed" << std::endl;
}

int main(){
    testBasicAllocation();
    testMemoryExhaustion();
    testAlignment();
    return 0;
}