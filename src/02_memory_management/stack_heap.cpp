/**
 * C++ Memory Management - Stack vs Heap, Pointers, References
 * Demonstrates memory allocation, deallocation, and memory layout
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

namespace memory {

// Global variable (stored in data/BSS segment)
int global_var = 100;

// Stack vs Heap demonstration
void demonstrate_stack_heap() {
    std::cout << "\n=== Stack vs Heap Memory ===\n";
    
    // Stack allocation
    int stack_var = 42;              // On stack
    char stack_array[100];           // On stack
    std::string stack_string = "Hello"; // Object on stack, but may have heap data
    
    std::cout << "Stack variable address: " << &stack_var << "\n";
    std::cout << "Stack array address: " << stack_array << "\n";
    std::cout << "Global variable address: " << &global_var << "\n";
    
    // Heap allocation (C-style - not recommended)
    int* heap_int = (int*)malloc(sizeof(int));
    *heap_int = 84;
    std::cout << "Heap int address: " << heap_int << ", value: " << *heap_int << "\n";
    free(heap_int);
    
    // Heap allocation (C++-style)
    int* cpp_heap_int = new int(168);
    std::cout << "C++ heap int address: " << cpp_heap_int << ", value: " << *cpp_heap_int << "\n";
    delete cpp_heap_int;
    
    // Dynamic array allocation
    int* dynamic_array = new int[5]{1, 2, 3, 4, 5};
    std::cout << "Dynamic array: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dynamic_array[i] << " ";
    }
    std::cout << "\n";
    delete[] dynamic_array;
    
    // Compare addresses to show memory layout
    std::cout << "\nMemory Layout Analysis:\n";
    int local1 = 1, local2 = 2;
    std::cout << "Local1 address: " << &local1 << "\n";
    std::cout << "Local2 address: " << &local2 << "\n";
    std::cout << "Difference: " << (char*)&local1 - (char*)&local2 << " bytes\n";
}

// Pointer and reference examples
void demonstrate_pointers_references() {
    std::cout << "\n=== Pointers and References ===\n";
    
    int value = 100;
    
    // Pointers
    int* ptr = &value;
    int** double_ptr = &ptr;
    
    std::cout << "Value: " << value << " (address: " << &value << ")\n";
    std::cout << "Pointer: " << ptr << " (points to: " << *ptr << ")\n";
    std::cout << "Double pointer: " << double_ptr << " (points to: " << *double_ptr << ", which points to: " << **double_ptr << ")\n";
    
    // References
    int& ref = value;
    std::cout << "Reference: " << ref << " (same address: " << &ref << ")\n";
    
    // Modifying through pointer and reference
    *ptr = 200;
    std::cout << "After *ptr = 200, value = " << value << "\n";
    
    ref = 300;
    std::cout << "After ref = 300, value = " << value << "\n";
    
    // Null pointer
    int* null_ptr = nullptr;
    if (null_ptr == nullptr) {
        std::cout << "Null pointer safely detected\n";
    }
    
    // Pointer arithmetic
    int arr[] = {10, 20, 30, 40, 50};
    int* arr_ptr = arr;
    
    std::cout << "Array pointer arithmetic:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  arr_ptr[" << i << "] = " << arr_ptr[i] 
                  << " (address: " << (arr_ptr + i) << ")\n";
    }
}

// Memory alignment demonstration
void demonstrate_memory_alignment() {
    std::cout << "\n=== Memory Alignment ===\n";
    
    struct UnalignedStruct {
        char c1;     // 1 byte
        int i;       // 4 bytes
        char c2;     // 1 byte
        double d;    // 8 bytes
    };
    
    struct AlignedStruct {
        double d;    // 8 bytes
        int i;       // 4 bytes
        char c1;     // 1 byte
        char c2;     // 1 byte
    };
    
    std::cout << "Size of char: " << sizeof(char) << "\n";
    std::cout << "Size of int: " << sizeof(int) << "\n";
    std::cout << "Size of double: " << sizeof(double) << "\n";
    std::cout << "Size of UnalignedStruct: " << sizeof(UnalignedStruct) << "\n";
    std::cout << "Size of AlignedStruct: " << sizeof(AlignedStruct) << "\n";
    
    // Show member offsets
    UnalignedStruct unaligned;
    std::cout << "\nUnalignedStruct member offsets:\n";
    std::cout << "  c1 offset: " << (char*)&unaligned.c1 - (char*)&unaligned << "\n";
    std::cout << "  i offset: " << (char*)&unaligned.i - (char*)&unaligned << "\n";
    std::cout << "  c2 offset: " << (char*)&unaligned.c2 - (char*)&unaligned << "\n";
    std::cout << "  d offset: " << (char*)&unaligned.d - (char*)&unaligned << "\n";
}

// Stack overflow demonstration (careful!)
void demonstrate_stack_concepts() {
    std::cout << "\n=== Stack Concepts ===\n";
    
    // Show stack growth
    static int call_depth = 0;
    if (call_depth < 3) {
        int local_var = call_depth;
        std::cout << "Call depth " << call_depth 
                  << ", local_var address: " << &local_var << "\n";
        call_depth++;
        demonstrate_stack_concepts(); // Recursive call
        call_depth--;
    }
    
    // Large stack allocation
    [[maybe_unused]] const size_t large_size = 1024 * 1024; // 1MB
    try {
        // Be careful with this in real code!
        char large_array[1000]; // Smaller for safety
        std::cout << "Large stack array allocated at: " << (void*)large_array << "\n";
        
        // Initialize to prevent optimization
        large_array[0] = 'A';
        large_array[999] = 'Z';
        std::cout << "Array initialized: " << large_array[0] << " to " << large_array[999] << "\n";
    } catch (...) {
        std::cout << "Stack overflow caught!\n";
    }
}

// Memory pool demonstration
class SimpleMemoryPool {
private:
    char* pool_;
    size_t size_;
    size_t used_;
    
public:
    explicit SimpleMemoryPool(size_t size) : size_(size), used_(0) {
        pool_ = new char[size];
        std::cout << "Memory pool created: " << size << " bytes at " << (void*)pool_ << "\n";
    }
    
    ~SimpleMemoryPool() {
        delete[] pool_;
        std::cout << "Memory pool destroyed\n";
    }
    
    void* allocate(size_t bytes) {
        if (used_ + bytes > size_) {
            std::cout << "Pool allocation failed: not enough space\n";
            return nullptr;
        }
        
        void* ptr = pool_ + used_;
        used_ += bytes;
        std::cout << "Pool allocated " << bytes << " bytes at " << ptr << "\n";
        return ptr;
    }
    
    void reset() {
        used_ = 0;
        std::cout << "Pool reset\n";
    }
    
    size_t available() const { return size_ - used_; }
    size_t total_size() const { return size_; }
};

void demonstrate_memory_pool() {
    std::cout << "\n=== Memory Pool ===\n";
    
    SimpleMemoryPool pool(1024);
    
    // Allocate some memory
    int* int_array = static_cast<int*>(pool.allocate(10 * sizeof(int)));
    if (int_array) {
        for (int i = 0; i < 10; ++i) {
            int_array[i] = i * i;
        }
        std::cout << "Initialized array in pool: ";
        for (int i = 0; i < 10; ++i) {
            std::cout << int_array[i] << " ";
        }
        std::cout << "\n";
    }
    
    char* char_array = static_cast<char*>(pool.allocate(100));
    if (char_array) {
        strcpy(char_array, "Hello from memory pool!");
        std::cout << "String in pool: " << char_array << "\n";
    }
    
    std::cout << "Pool usage: " << (pool.total_size() - pool.available()) 
              << "/" << pool.total_size() << " bytes\n";
}

// Memory debugging helpers
class LeakTracker {
public:
    static int allocations;
    static int deallocations;
    
    static void* operator new(size_t size) {
        allocations++;
        void* ptr = malloc(size);
        std::cout << "Allocated " << size << " bytes at " << ptr << "\n";
        return ptr;
    }
    
    static void operator delete(void* ptr) {
        deallocations++;
        std::cout << "Deallocated memory at " << ptr << "\n";
        free(ptr);
    }
};

int LeakTracker::allocations = 0;
int LeakTracker::deallocations = 0;

void demonstrate_memory_debugging() {
    std::cout << "\n=== Memory Debugging Helpers ===\n";
    
    std::cout << "Before allocations - Count: " << LeakTracker::allocations << "\n";
    
    LeakTracker* obj1 = new LeakTracker();
    LeakTracker* obj2 = new LeakTracker();
    delete obj1;
    
    std::cout << "Allocations: " << LeakTracker::allocations 
              << ", Deallocations: " << LeakTracker::deallocations << "\n";
    
    // Intentionally not deleting obj2 to show leak detection
    if (LeakTracker::allocations != LeakTracker::deallocations) {
        std::cout << "Memory leak detected!\n";
    }
    
    delete obj2; // Clean up for demonstration
}

} // namespace memory

int main() {
    std::cout << "=== C++ Memory Management - Stack and Heap ===\n";
    
    memory::demonstrate_stack_heap();
    memory::demonstrate_pointers_references();
    memory::demonstrate_memory_alignment();
    memory::demonstrate_stack_concepts();
    memory::demonstrate_memory_pool();
    memory::demonstrate_memory_debugging();
    
    return 0;
}