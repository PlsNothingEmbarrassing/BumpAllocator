#include <iostream>
#include "BumpDown.hpp"
#include <cassert>


#include <chrono>
#include <functional>

#include <tuple>
#include <thread>
using namespace std;

template<typename Function, typename ... Args>
auto benchmark(Function fn, Args&&...args){
    // start time
    auto start = chrono::high_resolution_clock::now();

    //invoke function
    fn(forward<Args>(args)...);
    
    // end time
    auto end = chrono::high_resolution_clock::now();

    // calculate result
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

template<typename Function, typename... Args>
void report_time(const std::string& fn_name, Function fn, Args&&... args){
    // Ouput benchmarks results
    auto duration = benchmark(fn, forward<Args>(args)...);
    cout << "Time taken by " << fn_name << ": " << duration << " ms";
}
void testBasicAllocation() {
    BumpDown<1024> allocator; // 1024 bytes of mem

    int* intPtr = allocator.alloc<int>();
    assert(intPtr != nullptr);

    double* doublePtr = allocator.alloc<double>();
    assert(doublePtr != nullptr);

    cout << " Test Basic Allocation: Passed" << std::endl;
}


void testMemoryExhaustion() {
    BumpDown<64> allocator; // small size to test exhaustion

    int* ptr1 = allocator.alloc<int>(10); // Allocating 10 ints
    assert(ptr1 != nullptr);

    int* ptr2 = allocator.alloc<int>(100); // Should fail as there isnt space
    assert(ptr2 == nullptr);

    cout << " Test Memory Exhaustion: Passed" << std::endl;
}

void testAlignment() {
    BumpDown<1024> allocator;

    // Assuming int requires 4 byte alignment
    int* intPtr = allocator.alloc<int>();
    assert(reinterpret_cast<uintptr_t>(intPtr) % alignof(int) == 0);

    // Assuming double requires 8 byte alignment
    double* doublePtr = allocator.alloc<double>();
    assert(reinterpret_cast<uintptr_t>(doublePtr) % alignof(double) == 0);

    cout << " Test Alignment: Passed" << endl;
}

void testDeallocation(){
    BumpDown<1024> allocator;

    auto* ptr1 = allocator.alloc<int>();
    auto* ptr2 = allocator.alloc<double>(5);
    auto* ptr3 = allocator.alloc<char>(10);
    // Check allocations are successful
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);
    assert(ptr3 != nullptr);

    size_t numAllocations = 3;
    // Deallocate allocations
    for (size_t i = 0; i < numAllocations; ++i){
        allocator.dealloc();
    }

    // Check if alloc is reset
    assert(allocator.getPtrPosition() == 0 || allocator.getPtrPosition() == 1024);
    // If next is in the correct position then deallocations successful
    cout << " Deallocated successfully" << endl;
}

int main(){
    report_time("testBasicAllocation", testBasicAllocation);
    report_time("testMemoryExhaustion", testMemoryExhaustion);
    report_time("testAlignment", testAlignment);
    report_time("testDeallocation", testDeallocation);
    
    return 0;
}