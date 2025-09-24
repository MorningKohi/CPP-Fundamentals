/**
 * C++ Functions - Modern Function Features and Best Practices
 * Covers function overloading, templates, lambdas, and C++17/20 features
 */

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <optional>
#include <chrono>

namespace functions {

// Basic function overloading
void print(int value) {
    std::cout << "Integer: " << value << "\n";
}

void print(double value) {
    std::cout << "Double: " << value << "\n";
}

void print(const std::string& value) {
    std::cout << "String: " << value << "\n";
}

// Function templates
template<typename T>
void print_template(T value) {
    std::cout << "Template: " << value << "\n";
}

// Variadic templates (C++11)
template<typename T>
void print_variadic(T&& t) {
    std::cout << std::forward<T>(t) << "\n";
}

template<typename T, typename... Args>
void print_variadic(T&& t, Args&&... args) {
    std::cout << std::forward<T>(t) << " ";
    print_variadic(std::forward<Args>(args)...);
}

// Function with default arguments
void configure_system(
    bool enable_logging = true,
    int max_threads = 4,
    double timeout = 30.0,
    const std::string& config_file = "default.cfg"
) {
    std::cout << "System configuration:\n";
    std::cout << "  Logging: " << std::boolalpha << enable_logging << "\n";
    std::cout << "  Max threads: " << max_threads << "\n";
    std::cout << "  Timeout: " << timeout << "s\n";
    std::cout << "  Config file: " << config_file << "\n";
}

// Return type deduction (C++14)
auto calculate_area(double radius) {
    return 3.14159 * radius * radius;
}

// Trailing return type (C++11)
template<typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b) {
    return a * b;
}

// Constexpr functions (C++11/14/17)
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Lambda expressions (C++11/14/17/20)
void demonstrate_lambdas() {
    std::cout << "\n=== Lambda Expressions ===\n";
    
    // Basic lambda
    auto simple_lambda = []() {
        std::cout << "Simple lambda called\n";
    };
    simple_lambda();
    
    // Lambda with parameters
    auto add = [](int a, int b) {
        return a + b;
    };
    std::cout << "5 + 3 = " << add(5, 3) << "\n";
    
    // Lambda with capture
    int multiplier = 10;
    auto multiply_by_ten = [multiplier](int value) {
        return value * multiplier;
    };
    std::cout << "7 * 10 = " << multiply_by_ten(7) << "\n";
    
    // Lambda with mutable capture
    auto counter = [count = 0]() mutable {
        return ++count;
    };
    std::cout << "Counter: " << counter() << ", " << counter() << ", " << counter() << "\n";
    
    // Generic lambda (C++14)
    auto generic_add = [](auto a, auto b) {
        return a + b;
    };
    std::cout << "Generic add: " << generic_add(5, 3) << " and " << generic_add(5.5, 3.2) << "\n";
    
    // Lambda with init capture (C++14)
    auto unique_ptr_lambda = [ptr = std::make_unique<int>(42)](){ 
        return *ptr; 
    };
    std::cout << "Unique ptr lambda: " << unique_ptr_lambda() << "\n";
    
    // Template lambda (C++20)
    auto template_lambda = []<typename T>(T value) {
        std::cout << "Template lambda with " << typeid(T).name() << ": " << value << "\n";
    };
    template_lambda(42);
    template_lambda(3.14);
}

// Function objects and std::function
class Multiplier {
private:
    int factor_;
public:
    explicit Multiplier(int factor) : factor_(factor) {}
    
    int operator()(int value) const {
        return value * factor_;
    }
};

void demonstrate_function_objects() {
    std::cout << "\n=== Function Objects and std::function ===\n";
    
    // Function object (functor)
    Multiplier times_five(5);
    std::cout << "Function object: " << times_five(8) << "\n";
    
    // std::function with various callables
    std::vector<std::function<int(int)>> operations;
    
    // Regular function
    operations.emplace_back([](int x) { return x * 2; });
    
    // Function object
    operations.emplace_back(Multiplier(3));
    
    // Lambda
    operations.emplace_back([](int x) { return x + 10; });
    
    int value = 5;
    std::cout << "Applying operations to " << value << ":\n";
    for (size_t i = 0; i < operations.size(); ++i) {
        std::cout << "  Operation " << i + 1 << ": " << operations[i](value) << "\n";
    }
}

// Higher-order functions
template<typename Container, typename Predicate>
auto filter(const Container& container, Predicate pred) {
    Container result;
    std::copy_if(container.begin(), container.end(), 
                 std::back_inserter(result), pred);
    return result;
}

template<typename Container, typename Transform>
auto map(const Container& container, Transform transform) {
    using ValueType = std::decay_t<decltype(transform(*container.begin()))>;
    std::vector<ValueType> result;
    std::transform(container.begin(), container.end(),
                   std::back_inserter(result), transform);
    return result;
}

template<typename Container, typename BinaryOp>
auto reduce(const Container& container, BinaryOp op) -> std::optional<typename Container::value_type> {
    if (container.empty()) return std::nullopt;
    
    auto result = *container.begin();
    for (auto it = std::next(container.begin()); it != container.end(); ++it) {
        result = op(result, *it);
    }
    return result;
}

void demonstrate_higher_order() {
    std::cout << "\n=== Higher-Order Functions ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Filter even numbers
    auto evens = filter(numbers, [](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << "\n";
    
    // Map to squares
    auto squares = map(numbers, [](int n) { return n * n; });
    std::cout << "Squares: ";
    for (int n : squares) std::cout << n << " ";
    std::cout << "\n";
    
    // Reduce to sum
    auto sum = reduce(numbers, [](int a, int b) { return a + b; });
    if (sum) {
        std::cout << "Sum: " << *sum << "\n";
    }
    
    // Chaining operations
    auto result = map(
        filter(numbers, [](int n) { return n % 2 == 0; }),
        [](int n) { return n * n; }
    );
    std::cout << "Squares of evens: ";
    for (int n : result) std::cout << n << " ";
    std::cout << "\n";
}

// Function with perfect forwarding
template<typename F, typename... Args>
auto time_function(F&& func, Args&&... args) -> decltype(func(std::forward<Args>(args)...)) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Function executed in " << duration.count() << " microseconds\n";
    
    return result;
}

// Recursive functions
int fibonacci_recursive(int n) {
    if (n <= 1) return n;
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

// Tail recursion
int fibonacci_tail_helper(int n, int a, int b) {
    if (n == 0) return a;
    if (n == 1) return b;
    return fibonacci_tail_helper(n - 1, b, a + b);
}

int fibonacci_tail(int n) {
    return fibonacci_tail_helper(n, 0, 1);
}

void demonstrate_recursion() {
    std::cout << "\n=== Recursive Functions ===\n";
    
    int n = 10;
    std::cout << "Fibonacci(" << n << ") recursive: " << fibonacci_recursive(n) << "\n";
    std::cout << "Fibonacci(" << n << ") tail recursive: " << fibonacci_tail(n) << "\n";
    
    // Constexpr evaluation at compile time
    constexpr int fact_5 = factorial(5);
    constexpr bool is_17_prime = is_prime(17);
    
    std::cout << "factorial(5) = " << fact_5 << "\n";
    std::cout << "is_prime(17) = " << std::boolalpha << is_17_prime << "\n";
}

} // namespace functions

int main() {
    std::cout << "=== C++ Functions Demonstration ===\n";
    
    // Function overloading
    std::cout << "\n=== Function Overloading ===\n";
    functions::print(42);
    functions::print(3.14);
    functions::print("Hello World");
    
    // Template functions
    std::cout << "\n=== Template Functions ===\n";
    functions::print_template(100);
    functions::print_template(2.718);
    functions::print_template(std::string("Template"));
    
    // Variadic templates
    std::cout << "\n=== Variadic Templates ===\n";
    functions::print_variadic("Multiple", "arguments", 42, 3.14, "here");
    
    // Default arguments
    std::cout << "\n=== Default Arguments ===\n";
    functions::configure_system();
    functions::configure_system(false, 8, 60.0, "custom.cfg");
    
    // Auto return type
    std::cout << "\n=== Auto Return Type ===\n";
    std::cout << "Circle area: " << functions::calculate_area(5.0) << "\n";
    std::cout << "Multiply result: " << functions::multiply(5, 3.14) << "\n";
    
    // Lambda demonstrations
    functions::demonstrate_lambdas();
    functions::demonstrate_function_objects();
    functions::demonstrate_higher_order();
    functions::demonstrate_recursion();
    
    return 0;
}