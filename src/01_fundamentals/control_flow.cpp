/**
 * C++ Control Flow - Modern Control Structures and Pattern Matching
 * Demonstrates loops, conditionals, and modern control flow features
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <ranges>
#include <random>
#include <variant>

namespace controlflow {

// Traditional control structures
void demonstrate_basic_control() {
    std::cout << "\n=== Basic Control Structures ===\n";
    
    // If-else with initializer (C++17)
    if (auto value = 42; value > 0) {
        std::cout << "Value " << value << " is positive\n";
    } else if (value < 0) {
        std::cout << "Value " << value << " is negative\n";
    } else {
        std::cout << "Value is zero\n";
    }
    
    // Switch with initializer (C++17)
    switch (auto day = 3; day) {
        case 1: case 2: case 3: case 4: case 5:
            std::cout << "Weekday " << day << "\n";
            break;
        case 6: case 7:
            std::cout << "Weekend day " << day << "\n";
            break;
        default:
            std::cout << "Invalid day\n";
    }
}

// Modern loop variations
void demonstrate_loops() {
    std::cout << "\n=== Modern Loop Variations ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Range-based for loop (C++11)
    std::cout << "Range-based for: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    
    // Range-based for with index simulation (C++20 ranges not widely available)
    std::cout << "With index: ";
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << "[" << i << "]=" << numbers[i] << " ";
    }
    std::cout << "\n";
    
    // Traditional for loop
    std::cout << "Traditional for: ";
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";
    
    // While loop with condition at the end
    std::cout << "While loop: ";
    auto it = numbers.begin();
    while (it != numbers.end()) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << "\n";
    
    // Do-while loop
    std::cout << "Do-while countdown: ";
    int countdown = 5;
    do {
        std::cout << countdown << " ";
        --countdown;
    } while (countdown > 0);
    std::cout << "\n";
}

// Algorithm-based control flow
void demonstrate_algorithm_control() {
    std::cout << "\n=== Algorithm-Based Control Flow ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // std::for_each
    std::cout << "std::for_each: ";
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << n * 2 << " ";
    });
    std::cout << "\n";
    
    // Conditional processing
    std::cout << "Even numbers: ";
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        if (n % 2 == 0) {
            std::cout << n << " ";
        }
    });
    std::cout << "\n";
    
    // std::find_if for early termination
    auto found = std::find_if(numbers.begin(), numbers.end(), [](int n) {
        return n > 5;
    });
    
    if (found != numbers.end()) {
        std::cout << "First number > 5: " << *found << "\n";
    }
    
    // std::any_of, std::all_of, std::none_of
    bool has_even = std::any_of(numbers.begin(), numbers.end(), [](int n) { return n % 2 == 0; });
    bool all_positive = std::all_of(numbers.begin(), numbers.end(), [](int n) { return n > 0; });
    bool none_negative = std::none_of(numbers.begin(), numbers.end(), [](int n) { return n < 0; });
    
    std::cout << "Has even: " << std::boolalpha << has_even << "\n";
    std::cout << "All positive: " << all_positive << "\n";
    std::cout << "None negative: " << none_negative << "\n";
}

// Exception handling control flow
void demonstrate_exception_control() {
    std::cout << "\n=== Exception Control Flow ===\n";
    
    // Basic try-catch
    auto safe_divide = [](double a, double b) {
        if (b == 0.0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    };
    
        try {
        double result1 = safe_divide(10.0, 2.0);
        std::cout << "10.0 / 2.0 = " << result1 << "\n";
        
        // This will throw - result2 declared but not used is intentional
        [[maybe_unused]] double result2 = safe_divide(10.0, 0.0);
        std::cout << "This won't be printed\n";
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Caught general exception: " << e.what() << "\n";
    }
    catch (...) {
        std::cout << "Caught unknown exception\n";
    }
    
    // RAII and exception safety
    std::cout << "Exception handling with RAII demonstrated\n";
}

// Modern switch alternatives (visitor pattern)
class ShapeVariant {
public:
    struct Circle { double radius; };
    struct Rectangle { double width, height; };
    struct Triangle { double base, height; };
    
    using Shape = std::variant<Circle, Rectangle, Triangle>;
    
    static void process_shape(const Shape& shape) {
        std::visit([](const auto& s) {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, Circle>) {
                std::cout << "Processing circle with radius " << s.radius << "\n";
            }
            else if constexpr (std::is_same_v<T, Rectangle>) {
                std::cout << "Processing rectangle " << s.width << "x" << s.height << "\n";
            }
            else if constexpr (std::is_same_v<T, Triangle>) {
                std::cout << "Processing triangle base=" << s.base << " height=" << s.height << "\n";
            }
        }, shape);
    }
};

void demonstrate_pattern_matching() {
    std::cout << "\n=== Pattern Matching with std::variant ===\n";
    
    std::vector<ShapeVariant::Shape> shapes = {
        ShapeVariant::Circle{5.0},
        ShapeVariant::Rectangle{4.0, 3.0},
        ShapeVariant::Triangle{6.0, 4.0}
    };
    
    for (const auto& shape : shapes) {
        ShapeVariant::process_shape(shape);
    }
}

// Coroutines (C++20) - basic example
#if __cpp_lib_coroutines
#include <coroutine>

struct Generator {
    struct promise_type {
        int current_value;
        auto get_return_object() { return Generator{handle_type::from_promise(*this)}; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() {}
        auto yield_value(int value) {
            current_value = value;
            return std::suspend_always{};
        }
        void return_void() {}
    };
    
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;
    
    Generator(handle_type h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }
    
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : coro(other.coro) { other.coro = {}; }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (coro) coro.destroy();
            coro = other.coro;
            other.coro = {};
        }
        return *this;
    }
    
    bool next() {
        coro.resume();
        return !coro.done();
    }
    
    int value() { return coro.promise().current_value; }
};

Generator fibonacci_generator(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        int temp = a;
        a = b;
        b = temp + b;
    }
}

void demonstrate_coroutines() {
    std::cout << "\n=== Coroutines (C++20) ===\n";
    auto fib = fibonacci_generator(10);
    std::cout << "Fibonacci sequence: ";
    while (fib.next()) {
        std::cout << fib.value() << " ";
    }
    std::cout << "\n";
}
#endif

} // namespace controlflow

int main() {
    std::cout << "=== C++ Control Flow Demonstration ===\n";
    
    controlflow::demonstrate_basic_control();
    controlflow::demonstrate_loops();
    controlflow::demonstrate_algorithm_control();
    controlflow::demonstrate_exception_control();
    controlflow::demonstrate_pattern_matching();
    
#if __cpp_lib_coroutines
    controlflow::demonstrate_coroutines();
#endif
    
    return 0;
}