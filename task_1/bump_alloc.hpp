#include <iostream>
// Size allocated to allocator
template <size_t Size>
class BumpAllocator{
    private:
        char heap[Size];
        size_t next = 0;
        int alloc_count = 0;        
    public:
        BumpAllocator() = default;
        // Don't allow assignment
        BumpAllocator& operator=(const BumpAllocator&) = delete;
        template <typename T>
        T* alloc(size_t N = 1){
            size_t alignment = alignof(T);
            size_t required_size = N * sizeof(T);
            // align next and add padding if needed
            size_t current_alignment_offset = next % alignment;
            size_t padding = (alignment - current_alignment_offset) % alignment;
            size_t aligned_next = next + padding;
            
            // Check for overflow
            if(aligned_next < next || aligned_next + required_size > Size){
                return nullptr;
            }            
            
            /* Use reinterpret cast as we are just treating the memory location as different type not doing type conversions. Pointer reinterpretation instead of type conversion.*/
            T* result = reinterpret_cast<T*>(heap + aligned_next);
            // Move next to unallocated space partition
            next = aligned_next + required_size;
            alloc_count++;
            return result;
            
        }
        void dealloc(){
            if(alloc_count > 0){
                alloc_count--;
                if(alloc_count == 0){
                    next = 0;
                }
            }
            next = 0;
        }
        
        size_t getPtrPosition()const{
            return next;
        }
};
