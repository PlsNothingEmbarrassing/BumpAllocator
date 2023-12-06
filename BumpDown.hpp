#include <iostream>
// Size allocated to allocator
template <size_t Size>
class BumpDown{
    private:
    // 
        char heap[Size];
        size_t next = Size;
        int alloc_count = 0;        
    public:
        BumpDown() = default;
        // Don't allow assignment
        BumpDown& operator=(const BumpDown&) = delete;
        // Using template function 
        template <typename T>
        T* alloc(size_t N = 1){
            size_t alignment = alignof(T);
            size_t required_size = N * sizeof(T);
            // align next ptr 
            size_t current_alignment_offset = next % alignment;
            if(current_alignment_offset != 0){
               next -= current_alignment_offset;
            }
            size_t aligned_next = next - required_size;         
            
            
            // Check for underflow
            if(aligned_next > next || aligned_next < sizeof(heap) - Size){
                return nullptr;
            }
            // update next            
            next = aligned_next;
            /* Use reinterpret cast as we are just treating the memory location as different type not doing type conversions. Pointer reinterpretation instead of type conversion.*/
            T* result = reinterpret_cast<T*>(heap + aligned_next);
            // Move next to unallocated space partition
            
            alloc_count++;
            return result;
            
        }
        void dealloc(){
            if(alloc_count > 0){
                alloc_count--;
                if(alloc_count == 0){
                    next = Size; // reset pointer if no allocations left
                }
            }
            
        }
        // Return next value for testing
        size_t getPtrPosition()const{
            return next;
        }

        int getAllocCount() const
        {
            return alloc_count;
        }
};