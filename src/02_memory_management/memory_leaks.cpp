/**
 * C++ Memory Leaks Detection and Prevention
 * Demonstrates common memory leak patterns and prevention techniques
 */

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>

namespace memoryleaks {

// Memory leak examples and fixes
class ResourceManager {
private:
    std::string* data_;
    size_t size_;
    
public:
    // BAD: No rule of three/five
    ResourceManager(size_t size) : size_(size) {
        data_ = new std::string[size];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = "Item " + std::to_string(i);
        }
        std::cout << "ResourceManager created with " << size_ << " items\n";
    }
    
    // GOOD: Proper destructor
    ~ResourceManager() {
        delete[] data_;
        std::cout << "ResourceManager destroyed\n";
    }
    
    // GOOD: Copy constructor (deep copy)
    ResourceManager(const ResourceManager& other) : size_(other.size_) {
        data_ = new std::string[size_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
        std::cout << "ResourceManager copy constructor called\n";
    }
    
    // GOOD: Copy assignment operator
    ResourceManager& operator=(const ResourceManager& other) {
        if (this != &other) {
            delete[] data_; // Clean up existing resources
            
            size_ = other.size_;
            data_ = new std::string[size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        std::cout << "ResourceManager copy assignment called\n";
        return *this;
    }
    
    // GOOD: Move constructor (C++11)
    ResourceManager(ResourceManager&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "ResourceManager move constructor called\n";
    }
    
    // GOOD: Move assignment operator (C++11)
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            
            data_ = other.data_;
            size_ = other.size_;
            
            other.data_ = nullptr;
            other.size_ = 0;
        }
        std::cout << "ResourceManager move assignment called\n";
        return *this;
    }
    
    void display() const {
        if (data_) {
            std::cout << "Resources: ";
            for (size_t i = 0; i < size_ && i < 3; ++i) {
                std::cout << data_[i] << " ";
            }
            if (size_ > 3) std::cout << "... (" << size_ << " total)";
            std::cout << "\n";
        } else {
            std::cout << "No resources (moved)\n";
        }
    }
};

// Common memory leak pattern 1: Double deletion
void demonstrate_double_deletion() {
    std::cout << "\n=== Double Deletion Prevention ===\n";
    
    int* ptr = new int(42);
    std::cout << "Created pointer: " << ptr << " with value: " << *ptr << "\n";
    
    delete ptr;
    ptr = nullptr; // IMPORTANT: Set to nullptr after deletion
    
    // This is safe now
    if (ptr) {
        delete ptr; // Won't execute
    } else {
        std::cout << "Pointer already null, safe from double deletion\n";
    }
    
    // Better approach with smart pointers
    {
        std::unique_ptr<int> smart_ptr = std::make_unique<int>(84);
        std::cout << "Smart pointer value: " << *smart_ptr << "\n";
        smart_ptr.reset(); // Explicit deletion, pointer becomes null
        
        // This is always safe
        if (smart_ptr) {
            std::cout << "Smart pointer is valid\n";
        } else {
            std::cout << "Smart pointer is null, no risk of double deletion\n";
        }
    }
}

// Common memory leak pattern 2: Exception safety
class ExceptionUnsafe {
private:
    int* data1_;
    int* data2_;
    
public:
    // BAD: Not exception safe
    ExceptionUnsafe(bool throw_exception) {
        data1_ = new int(100);
        
        if (throw_exception) {
            throw std::runtime_error("Construction failed");
            // data1_ leaks if exception thrown before data2_ allocation
        }
        
        data2_ = new int(200);
    }
    
    ~ExceptionUnsafe() {
        delete data1_;
        delete data2_;
    }
};

class ExceptionSafe {
private:
    std::unique_ptr<int> data1_;
    std::unique_ptr<int> data2_;
    
public:
    // GOOD: Exception safe with smart pointers
    ExceptionSafe(bool throw_exception) {
        data1_ = std::make_unique<int>(100);
        
        if (throw_exception) {
            throw std::runtime_error("Construction failed");
            // data1_ automatically cleaned up by unique_ptr
        }
        
        data2_ = std::make_unique<int>(200);
    }
    
    void display() const {
        std::cout << "Data1: " << *data1_ << ", Data2: " << *data2_ << "\n";
    }
};

void demonstrate_exception_safety() {
    std::cout << "\n=== Exception Safety ===\n";
    
    // Demonstrate exception safety with smart pointers
    try {
        ExceptionSafe safe_obj(false); // Won't throw
        safe_obj.display();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    
    try {
        ExceptionSafe safe_obj(true); // Will throw, but no leaks
        safe_obj.display();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << " (no memory leaks)\n";
    }
}

// Memory leak pattern 3: Circular references
struct Node {
    int value;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent; // Using weak_ptr to break cycles
    
    Node(int v) : value(v) {
        std::cout << "Node " << value << " created\n";
    }
    
    ~Node() {
        std::cout << "Node " << value << " destroyed\n";
    }
};

void demonstrate_circular_references() {
    std::cout << "\n=== Circular Reference Prevention ===\n";
    
    // Create nodes with proper weak_ptr usage
    {
        auto node1 = std::make_shared<Node>(1);
        auto node2 = std::make_shared<Node>(2);
        auto node3 = std::make_shared<Node>(3);
        
        // Create chain: 1 -> 2 -> 3
        node1->next = node2;
        node2->next = node3;
        
        // Set parent relationships (weak references)
        node2->parent = node1;
        node3->parent = node2;
        
        std::cout << "Node chain created\n";
        std::cout << "Node1 ref count: " << node1.use_count() << "\n";
        std::cout << "Node2 ref count: " << node2.use_count() << "\n";
        std::cout << "Node3 ref count: " << node3.use_count() << "\n";
        
    } // All nodes should be properly destroyed
    
    std::cout << "Node chain destroyed (no cycles)\n";
}

// Memory leak pattern 4: Container of raw pointers
void demonstrate_container_cleanup() {
    std::cout << "\n=== Container Cleanup ===\n";
    
    // BAD: Vector of raw pointers (potential leaks)
    std::vector<int*> bad_vector;
    for (int i = 0; i < 5; ++i) {
        bad_vector.push_back(new int(i));
    }
    
    std::cout << "Created vector of raw pointers\n";
    
    // Manual cleanup required
    for (auto* ptr : bad_vector) {
        delete ptr;
    }
    bad_vector.clear();
    std::cout << "Manually cleaned up raw pointers\n";
    
    // GOOD: Vector of smart pointers (automatic cleanup)
    std::vector<std::unique_ptr<int>> good_vector;
    for (int i = 0; i < 5; ++i) {
        good_vector.push_back(std::make_unique<int>(i));
    }
    
    std::cout << "Created vector of smart pointers\n";
    // Automatic cleanup when vector goes out of scope
}

// Memory usage tracking
class MemoryTracker {
private:
    static size_t total_allocated_;
    static size_t total_deallocated_;
    static size_t current_usage_;
    
public:
    static void* allocate(size_t size) {
        void* ptr = malloc(size);
        if (ptr) {
            total_allocated_ += size;
            current_usage_ += size;
            std::cout << "Allocated " << size << " bytes (total: " << current_usage_ << ")\n";
        }
        return ptr;
    }
    
    static void deallocate(void* ptr, size_t size) {
        if (ptr) {
            free(ptr);
            total_deallocated_ += size;
            current_usage_ -= size;
            std::cout << "Deallocated " << size << " bytes (total: " << current_usage_ << ")\n";
        }
    }
    
    static void report() {
        std::cout << "\n=== Memory Usage Report ===\n";
        std::cout << "Total allocated: " << total_allocated_ << " bytes\n";
        std::cout << "Total deallocated: " << total_deallocated_ << " bytes\n";
        std::cout << "Current usage: " << current_usage_ << " bytes\n";
        
        if (current_usage_ > 0) {
            std::cout << "WARNING: Memory leak detected!\n";
        } else {
            std::cout << "No memory leaks detected\n";
        }
    }
};

size_t MemoryTracker::total_allocated_ = 0;
size_t MemoryTracker::total_deallocated_ = 0;
size_t MemoryTracker::current_usage_ = 0;

// RAII wrapper for tracked memory
template<typename T>
class TrackedPtr {
private:
    T* ptr_;
    size_t size_;
    
public:
    explicit TrackedPtr(size_t count = 1) : size_(count * sizeof(T)) {
        ptr_ = static_cast<T*>(MemoryTracker::allocate(size_));
    }
    
    ~TrackedPtr() {
        if (ptr_) {
            MemoryTracker::deallocate(ptr_, size_);
        }
    }
    
    TrackedPtr(const TrackedPtr&) = delete;
    TrackedPtr& operator=(const TrackedPtr&) = delete;
    
    TrackedPtr(TrackedPtr&& other) noexcept : ptr_(other.ptr_), size_(other.size_) {
        other.ptr_ = nullptr;
        other.size_ = 0;
    }
    
    TrackedPtr& operator=(TrackedPtr&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                MemoryTracker::deallocate(ptr_, size_);
            }
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }
};

void demonstrate_memory_tracking() {
    std::cout << "\n=== Memory Tracking ===\n";
    
    {
        TrackedPtr<int> ptr1(1);
        if (ptr1) {
            *ptr1 = 42;
            std::cout << "Tracked pointer value: " << *ptr1 << "\n";
        }
        
        TrackedPtr<int> ptr2(10); // Array of 10 ints
        
        MemoryTracker::report();
        
    } // Automatic cleanup
    
    MemoryTracker::report();
}

} // namespace memoryleaks

int main() {
    std::cout << "=== C++ Memory Leaks Detection and Prevention ===\n";
    
    // Demonstrate rule of three/five
    std::cout << "\n=== Rule of Three/Five ===\n";
    {
        memoryleaks::ResourceManager rm1(5);
        rm1.display();
        
        memoryleaks::ResourceManager rm2 = rm1; // Copy constructor
        rm2.display();
        
        memoryleaks::ResourceManager rm3(3);
        rm3 = rm1; // Copy assignment
        rm3.display();
        
        memoryleaks::ResourceManager rm4 = std::move(rm1); // Move constructor
        rm1.display(); // Should show moved state
        rm4.display();
        
    } // All objects destroyed properly
    
    memoryleaks::demonstrate_double_deletion();
    memoryleaks::demonstrate_exception_safety();
    memoryleaks::demonstrate_circular_references();
    memoryleaks::demonstrate_container_cleanup();
    memoryleaks::demonstrate_memory_tracking();
    
    return 0;
}