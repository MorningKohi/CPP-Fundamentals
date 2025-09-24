/**
 * C++ Smart Pointers - Modern Memory Management
 * Demonstrates unique_ptr, shared_ptr, weak_ptr, and custom smart pointers
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace smartpointers {

// Resource class for demonstration
class Resource {
private:
    std::string name_;
    int value_;
    
public:
    Resource(const std::string& name, int value) 
        : name_(name), value_(value) {
        std::cout << "Resource created: " << name_ << " (" << value_ << ")\n";
    }
    
    ~Resource() {
        std::cout << "Resource destroyed: " << name_ << " (" << value_ << ")\n";
    }
    
    void use() const {
        std::cout << "Using resource: " << name_ << " with value " << value_ << "\n";
    }
    
    const std::string& name() const { return name_; }
    int value() const { return value_; }
    void set_value(int value) { value_ = value; }
};

// unique_ptr demonstration
void demonstrate_unique_ptr() {
    std::cout << "\n=== std::unique_ptr ===\n";
    
    // Creating unique_ptr
    std::unique_ptr<Resource> res1 = std::make_unique<Resource>("Resource1", 100);
    res1->use();
    
    // Moving ownership
    std::unique_ptr<Resource> res2 = std::move(res1);
    if (!res1) {
        std::cout << "res1 is now null after move\n";
    }
    res2->use();
    
    // unique_ptr with arrays
    std::unique_ptr<int[]> int_array = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        int_array[i] = i * i;
    }
    
    std::cout << "Array contents: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << int_array[i] << " ";
    }
    std::cout << "\n";
    
    // Custom deleter
    auto custom_deleter = [](Resource* ptr) {
        std::cout << "Custom deleter called for " << ptr->name() << "\n";
        delete ptr;
    };
    
    std::unique_ptr<Resource, decltype(custom_deleter)> 
        res3(new Resource("CustomDeleted", 200), custom_deleter);
    res3->use();
    
    // unique_ptr with factory function
    auto create_resource = [](const std::string& name, int value) {
        return std::make_unique<Resource>(name, value);
    };
    
    auto factory_res = create_resource("Factory", 300);
    factory_res->use();
    
} // All resources automatically cleaned up here

// shared_ptr demonstration
void demonstrate_shared_ptr() {
    std::cout << "\n=== std::shared_ptr ===\n";
    
    // Creating shared_ptr
    std::shared_ptr<Resource> shared1 = std::make_shared<Resource>("Shared1", 400);
    std::cout << "Reference count: " << shared1.use_count() << "\n";
    
    // Sharing ownership
    {
        std::shared_ptr<Resource> shared2 = shared1;
        std::cout << "Reference count after sharing: " << shared1.use_count() << "\n";
        
        std::shared_ptr<Resource> shared3 = shared1;
        std::cout << "Reference count with 3 owners: " << shared1.use_count() << "\n";
        
        shared2->use();
        shared3->use();
        
    } // shared2 and shared3 go out of scope
    
    std::cout << "Reference count after scope exit: " << shared1.use_count() << "\n";
    shared1->use();
    
    // shared_ptr with custom deleter
    std::shared_ptr<Resource> shared_custom(
        new Resource("SharedCustom", 500),
        [](Resource* ptr) {
            std::cout << "Shared custom deleter for " << ptr->name() << "\n";
            delete ptr;
        }
    );
    
    // Circular reference problem (and solution)
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
    
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    
    node1->next = node2;
    node2->parent = node1; // weak_ptr doesn't increase reference count
    
    std::cout << "Node1 ref count: " << node1.use_count() << "\n";
    std::cout << "Node2 ref count: " << node2.use_count() << "\n";
}

// weak_ptr demonstration  
void demonstrate_weak_ptr() {
    std::cout << "\n=== std::weak_ptr ===\n";
    
    std::weak_ptr<Resource> weak_res;
    
    {
        std::shared_ptr<Resource> shared_res = std::make_shared<Resource>("WeakTest", 600);
        weak_res = shared_res;
        
        std::cout << "Weak ptr expired: " << std::boolalpha << weak_res.expired() << "\n";
        
        // Lock to get shared_ptr
        if (auto locked = weak_res.lock()) {
            locked->use();
            std::cout << "Successfully locked weak_ptr\n";
        }
        
    } // shared_res goes out of scope and resource is destroyed
    
    std::cout << "Weak ptr expired after scope: " << std::boolalpha << weak_res.expired() << "\n";
    
    if (auto locked = weak_res.lock()) {
        locked->use();
    } else {
        std::cout << "Cannot lock expired weak_ptr\n";
    }
}

// Forward declaration
class Observer;

// Observer pattern with weak_ptr
class Subject {
private:
    std::vector<std::weak_ptr<Observer>> observers_;
    std::string state_;
    
public:
    void attach(std::shared_ptr<Observer> observer);
    void set_state(const std::string& state);
    void notify();
};

class Observer {
private:
    std::string name_;
    
public:
    explicit Observer(const std::string& name) : name_(name) {
        std::cout << "Observer " << name_ << " created\n";
    }
    
    ~Observer() {
        std::cout << "Observer " << name_ << " destroyed\n";
    }
    
    void update(const std::string& state) {
        std::cout << "Observer " << name_ << " received update: " << state << "\n";
    }
};

// Subject method implementations
void Subject::attach(std::shared_ptr<Observer> observer) {
    observers_.push_back(observer);
    std::cout << "Observer attached\n";
}

void Subject::set_state(const std::string& state) {
    state_ = state;
    notify();
}

void Subject::notify() {
    std::cout << "Notifying observers of state: " << state_ << "\n";
    
    // Clean up expired weak_ptrs and notify valid ones
    auto it = observers_.begin();
    while (it != observers_.end()) {
        if (auto observer = it->lock()) {
            observer->update(state_);
            ++it;
        } else {
            // Remove expired weak_ptr
            it = observers_.erase(it);
            std::cout << "Removed expired observer\n";
        }
    }
}

void demonstrate_observer_pattern() {
    std::cout << "\n=== Observer Pattern with weak_ptr ===\n";
    
    Subject subject;
    
    {
        auto observer1 = std::make_shared<Observer>("Observer1");
        auto observer2 = std::make_shared<Observer>("Observer2");
        
        subject.attach(observer1);
        subject.attach(observer2);
        
        subject.set_state("State1");
        
        // observer2 goes out of scope
    }
    
    subject.set_state("State2"); // Should clean up expired observer2
}

// Custom smart pointer implementation
template<typename T>
class SimpleUniquePtr {
private:
    T* ptr_;
    
public:
    explicit SimpleUniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
    
    // Delete copy constructor and assignment
    SimpleUniquePtr(const SimpleUniquePtr&) = delete;
    SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;
    
    // Move constructor and assignment
    SimpleUniquePtr(SimpleUniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    
    SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    ~SimpleUniquePtr() {
        reset();
    }
    
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    
    explicit operator bool() const { return ptr_ != nullptr; }
    
    T* get() const { return ptr_; }
    
    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
    
    void reset(T* new_ptr = nullptr) {
        delete ptr_;
        ptr_ = new_ptr;
    }
};

template<typename T, typename... Args>
SimpleUniquePtr<T> make_simple_unique(Args&&... args) {
    return SimpleUniquePtr<T>(new T(std::forward<Args>(args)...));
}

void demonstrate_custom_smart_ptr() {
    std::cout << "\n=== Custom Smart Pointer ===\n";
    
    auto custom_ptr = make_simple_unique<Resource>("Custom", 700);
    custom_ptr->use();
    
    // Move semantics
    auto moved_ptr = std::move(custom_ptr);
    if (!custom_ptr) {
        std::cout << "Original pointer is null after move\n";
    }
    moved_ptr->use();
    
    // Release ownership
    Resource* raw_ptr = moved_ptr.release();
    raw_ptr->use();
    delete raw_ptr; // Manual cleanup after release
}

// RAII with smart pointers
class FileManager {
private:
    std::unique_ptr<std::string> filename_;
    
public:
    explicit FileManager(const std::string& filename) 
        : filename_(std::make_unique<std::string>(filename)) {
        std::cout << "File manager created for: " << *filename_ << "\n";
    }
    
    void process() const {
        if (filename_) {
            std::cout << "Processing file: " << *filename_ << "\n";
        }
    }
    
    // Move-only class
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
    
    FileManager(FileManager&&) = default;
    FileManager& operator=(FileManager&&) = default;
};

void demonstrate_raii_smart_ptr() {
    std::cout << "\n=== RAII with Smart Pointers ===\n";
    
    {
        FileManager manager("data.txt");
        manager.process();
    } // Automatic cleanup
    
    std::cout << "FileManager automatically cleaned up\n";
}

} // namespace smartpointers

int main() {
    std::cout << "=== C++ Smart Pointers Demonstration ===\n";
    
    smartpointers::demonstrate_unique_ptr();
    smartpointers::demonstrate_shared_ptr();
    smartpointers::demonstrate_weak_ptr();
    smartpointers::demonstrate_observer_pattern();
    smartpointers::demonstrate_custom_smart_ptr();
    smartpointers::demonstrate_raii_smart_ptr();
    
    return 0;
}