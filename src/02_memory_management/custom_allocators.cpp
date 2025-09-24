/**
 * C++ Custom Allocators - Memory Pool and Custom Allocation Strategies
 * Demonstrates custom allocators, memory pools, and allocation policies
 */

#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <cassert>
#include <chrono>

namespace allocators {

// Simple memory pool allocator
template<size_t PoolSize>
class MemoryPool {
private:
    alignas(std::max_align_t) char pool_[PoolSize];
    char* next_free_;
    size_t remaining_;
    
public:
    MemoryPool() : next_free_(pool_), remaining_(PoolSize) {
        std::cout << "Memory pool created: " << PoolSize << " bytes at " 
                  << static_cast<void*>(pool_) << "\n";
    }
    
    ~MemoryPool() {
        std::cout << "Memory pool destroyed\n";
    }
    
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        // Align the allocation
        size_t aligned_size = (size + alignment - 1) & ~(alignment - 1);
        
        if (aligned_size > remaining_) {
            std::cout << "Pool allocation failed: need " << aligned_size 
                      << " bytes, have " << remaining_ << "\n";
            return nullptr;
        }
        
        // Align the pointer
        char* aligned_ptr = reinterpret_cast<char*>(
            (reinterpret_cast<uintptr_t>(next_free_) + alignment - 1) & ~(alignment - 1)
        );
        
        size_t actual_size = aligned_ptr - next_free_ + aligned_size;
        if (actual_size > remaining_) {
            std::cout << "Pool allocation failed after alignment\n";
            return nullptr;
        }
        
        void* result = aligned_ptr;
        next_free_ = aligned_ptr + aligned_size;
        remaining_ -= actual_size;
        
        std::cout << "Pool allocated " << size << " bytes (" << aligned_size 
                  << " aligned) at " << result << "\n";
        return result;
    }
    
    void reset() {
        next_free_ = pool_;
        remaining_ = PoolSize;
        std::cout << "Pool reset\n";
    }
    
    size_t available() const { return remaining_; }
    size_t used() const { return PoolSize - remaining_; }
    
    bool owns(void* ptr) const {
        return ptr >= pool_ && ptr < pool_ + PoolSize;
    }
};

// Custom allocator that uses memory pool
template<typename T, size_t PoolSize = 4096>
class PoolAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    
    template<typename U>
    struct rebind {
        using other = PoolAllocator<U, PoolSize>;
    };
    
private:
    static MemoryPool<PoolSize> pool_;
    
public:
    PoolAllocator() = default;
    
    template<typename U>
    PoolAllocator(const PoolAllocator<U, PoolSize>&) noexcept {}
    
    pointer allocate(size_type n) {
        size_t size = n * sizeof(T);
        void* ptr = pool_.allocate(size, alignof(T));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(ptr);
    }
    
    void deallocate(pointer ptr, size_type n) {
        // Pool allocator doesn't support individual deallocation
        // In a real implementation, you might track allocations
        (void)ptr; // Suppress unused parameter warning
        std::cout << "Pool deallocate called (no-op for " << n << " items)\n";
    }
    
    template<typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        new(ptr) U(std::forward<Args>(args)...);
    }
    
    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }
    
    static void reset_pool() {
        pool_.reset();
    }
    
    static size_t pool_usage() {
        return pool_.used();
    }
    
    template<typename U>
    bool operator==(const PoolAllocator<U, PoolSize>&) const noexcept {
        return true; // All pool allocators of same size are equivalent
    }
    
    template<typename U>
    bool operator!=(const PoolAllocator<U, PoolSize>& other) const noexcept {
        return !(*this == other);
    }
};

// Static member initialization
template<typename T, size_t PoolSize>
MemoryPool<PoolSize> PoolAllocator<T, PoolSize>::pool_;

// Stack allocator (LIFO)
template<size_t StackSize>
class StackAllocator {
private:
    alignas(std::max_align_t) char stack_[StackSize];
    char* top_;
    
public:
    StackAllocator() : top_(stack_) {
        std::cout << "Stack allocator created: " << StackSize << " bytes\n";
    }
    
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        // Align the allocation
        char* aligned_top = reinterpret_cast<char*>(
            (reinterpret_cast<uintptr_t>(top_) + alignment - 1) & ~(alignment - 1)
        );
        
        if (aligned_top + size > stack_ + StackSize) {
            std::cout << "Stack allocation failed\n";
            return nullptr;
        }
        
        void* result = aligned_top;
        top_ = aligned_top + size;
        
        std::cout << "Stack allocated " << size << " bytes at " << result << "\n";
        return result;
    }
    
    void deallocate_to(void* ptr) {
        if (ptr >= stack_ && ptr <= stack_ + StackSize) {
            top_ = static_cast<char*>(ptr);
            std::cout << "Stack deallocated to " << ptr << "\n";
        }
    }
    
    void reset() {
        top_ = stack_;
        std::cout << "Stack allocator reset\n";
    }
    
    size_t used() const {
        return top_ - stack_;
    }
    
    size_t available() const {
        return StackSize - used();
    }
};

// Debug allocator that tracks allocations
template<typename T>
class DebugAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    
    template<typename U>
    struct rebind {
        using other = DebugAllocator<U>;
    };
    
private:
    static size_t allocation_count_;
    static size_t deallocation_count_;
    static size_t bytes_allocated_;
    
public:
    DebugAllocator() = default;
    
    template<typename U>
    DebugAllocator(const DebugAllocator<U>&) noexcept {}
    
    pointer allocate(size_type n) {
        size_t size = n * sizeof(T);
        pointer ptr = static_cast<pointer>(std::malloc(size));
        if (!ptr) {
            throw std::bad_alloc();
        }
        
        allocation_count_++;
        bytes_allocated_ += size;
        
        std::cout << "Debug allocated " << n << " objects (" << size 
                  << " bytes) at " << ptr << " [total: " << allocation_count_ << "]\n";
        
        return ptr;
    }
    
    void deallocate(pointer ptr, size_type n) {
        deallocation_count_++;
        
        std::cout << "Debug deallocated " << n << " objects at " << static_cast<void*>(ptr)
                  << " [total: " << deallocation_count_ << "]\n";
        
        std::free(ptr);
    }
    
    static void report() {
        std::cout << "\n=== Debug Allocator Report ===\n";
        std::cout << "Allocations: " << allocation_count_ << "\n";
        std::cout << "Deallocations: " << deallocation_count_ << "\n";
        std::cout << "Bytes allocated: " << bytes_allocated_ << "\n";
        
        if (allocation_count_ != deallocation_count_) {
            std::cout << "WARNING: Allocation/Deallocation mismatch!\n";
        }
    }
    
    template<typename U>
    bool operator==(const DebugAllocator<U>&) const noexcept {
        return true;
    }
    
    template<typename U>
    bool operator!=(const DebugAllocator<U>& other) const noexcept {
        return !(*this == other);
    }
};

// Static member initialization
template<typename T>
size_t DebugAllocator<T>::allocation_count_ = 0;

template<typename T>
size_t DebugAllocator<T>::deallocation_count_ = 0;

template<typename T>
size_t DebugAllocator<T>::bytes_allocated_ = 0;

// Performance comparison
void demonstrate_allocator_performance() {
    std::cout << "\n=== Allocator Performance Comparison ===\n";
    
    const size_t num_operations = 10000;
    const size_t allocation_size = 64;
    
    // Standard allocator
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<void*> ptrs;
        for (size_t i = 0; i < num_operations; ++i) {
            ptrs.push_back(std::malloc(allocation_size));
        }
        for (void* ptr : ptrs) {
            std::free(ptr);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Standard allocator: " << std_duration.count() << " microseconds\n";
    
    // Pool allocator (simplified performance test)
    start = std::chrono::high_resolution_clock::now();
    {
        MemoryPool<1024 * 1024> pool; // 1MB pool
        std::vector<void*> ptrs;
        for (size_t i = 0; i < std::min(num_operations, size_t(1024 * 1024 / allocation_size)); ++i) {
            void* ptr = pool.allocate(allocation_size);
            if (ptr) ptrs.push_back(ptr);
        }
        // Pool doesn't support individual deallocation
    }
    end = std::chrono::high_resolution_clock::now();
    auto pool_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Pool allocator: " << pool_duration.count() << " microseconds\n";
    
    if (pool_duration < std_duration) {
        std::cout << "Pool allocator is faster by " 
                  << (std_duration.count() - pool_duration.count()) << " microseconds\n";
    }
}

// Usage examples
void demonstrate_pool_allocator() {
    std::cout << "\n=== Pool Allocator Usage ===\n";
    
    using PoolVector = std::vector<int, PoolAllocator<int, 1024>>;
    
    {
        PoolVector vec;
        vec.reserve(50);
        
        for (int i = 0; i < 20; ++i) {
            vec.push_back(i * i);
        }
        
        std::cout << "Vector contents: ";
        for (int value : vec) {
            std::cout << value << " ";
        }
        std::cout << "\n";
        
        std::cout << "Pool usage: " << PoolAllocator<int>::pool_usage() << " bytes\n";
    }
    
    // Reset pool for reuse
    PoolAllocator<int>::reset_pool();
}

void demonstrate_stack_allocator() {
    std::cout << "\n=== Stack Allocator Usage ===\n";
    
    StackAllocator<1024> stack_alloc;
    
    // Allocate some memory blocks
    [[maybe_unused]] void* ptr1 = stack_alloc.allocate(100);
    void* ptr2 = stack_alloc.allocate(200);
    [[maybe_unused]] void* ptr3 = stack_alloc.allocate(50);
    
    std::cout << "Stack usage: " << stack_alloc.used() 
              << "/" << (stack_alloc.used() + stack_alloc.available()) << " bytes\n";
    
    // Deallocate back to ptr2 (LIFO order)
    stack_alloc.deallocate_to(ptr2);
    std::cout << "After deallocation to ptr2, usage: " << stack_alloc.used() << " bytes\n";
    
    // Can allocate again
    void* ptr4 = stack_alloc.allocate(300);
    std::cout << "Allocated new block: " << ptr4 << "\n";
}

void demonstrate_debug_allocator() {
    std::cout << "\n=== Debug Allocator Usage ===\n";
    
    using DebugVector = std::vector<std::string, DebugAllocator<std::string>>;
    
    {
        DebugVector vec;
        vec.push_back("Hello");
        vec.push_back("World");
        vec.push_back("Debug");
        vec.push_back("Allocator");
        
        std::cout << "Vector contents: ";
        for (const auto& str : vec) {
            std::cout << str << " ";
        }
        std::cout << "\n";
    }
    
    DebugAllocator<std::string>::report();
}

} // namespace allocators

int main() {
    std::cout << "=== C++ Custom Allocators Demonstration ===\n";
    
    allocators::demonstrate_pool_allocator();
    allocators::demonstrate_stack_allocator();
    allocators::demonstrate_debug_allocator();
    allocators::demonstrate_allocator_performance();
    
    return 0;
}