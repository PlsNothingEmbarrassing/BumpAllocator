#include <iostream>
#include <BumpDown.hpp>
using namespace std;
int main()
{
    BumpDown<4> allocator;
    int *intPtr = allocator.alloc<int>(1);


}