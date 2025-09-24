/**
 * C++ Syntax Basics - Modern C++17/20/23 Features
 * Demonstrates fundamental syntax elements and modern features
 */

#include <iostream>
#include <string>
#include <string_view>
#include <optional>
#include <variant>
#include <vector>
#include <algorithm>
// #include <format> // C++20 feature - not widely available yet

namespace fundamentals {

// C++17 structured bindings
void demonstrate_structured_bindings() {
    std::cout << "\n=== Structured Bindings (C++17) ===\n";
    
    // With pairs
    auto pair = std::make_pair(42, "Hello");
    auto [number, text] = pair;
    std::cout << "Number: " << number << ", Text: " << text << "\n";
    
    // With arrays
    int arr[] = {1, 2, 3};
    auto [a, b, c] = arr;
    std::cout << "Array elements: " << a << ", " << b << ", " << c << "\n";
}

// C++17 if constexpr and template argument deduction
template<typename T>
void process_value(T value) {
    std::cout << "\n=== Template Processing ===\n";
    
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Processing integer: " << value << "\n";
    }
    else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Processing float: " << value << "\n";
    }
    else {
        std::cout << "Processing other type\n";
    }
}

// C++20 concepts (if available)
#if __cpp_concepts >= 201907L
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
void concept_demo(T value) {
    std::cout << "\n=== Concepts Demo (C++20) ===\n";
    std::cout << "Numeric value: " << value << "\n";
}
#endif

// C++17 std::optional
void demonstrate_optional() {
    std::cout << "\n=== std::optional (C++17) ===\n";
    
    auto safe_divide = [](double a, double b) -> std::optional<double> {
        if (b == 0.0) return std::nullopt;
        return a / b;
    };
    
    if (auto result = safe_divide(10.0, 2.0); result.has_value()) {
        std::cout << "10.0 / 2.0 = " << *result << "\n";
    }
    
    if (auto result = safe_divide(10.0, 0.0); !result.has_value()) {
        std::cout << "Division by zero handled safely\n";
    }
}

// C++17 std::variant
void demonstrate_variant() {
    std::cout << "\n=== std::variant (C++17) ===\n";
    
    std::variant<int, double, std::string> var;
    
    var = 42;
    std::cout << "Integer: " << std::get<int>(var) << "\n";
    
    var = 3.14;
    std::cout << "Double: " << std::get<double>(var) << "\n";
    
    var = std::string("Hello");
    std::cout << "String: " << std::get<std::string>(var) << "\n";
    
    // Visitor pattern with variant
    std::visit([](auto&& arg) {
        std::cout << "Visiting: " << arg << "\n";
    }, var);
}

// C++17 inline variables
inline constexpr double PI = 3.14159265359;

// C++20 designated initializers (if available)
struct Point {
    double x, y, z;
};

void demonstrate_designated_init() {
    std::cout << "\n=== Designated Initializers ===\n";
    Point p{.x = 1.0, .y = 2.0, .z = 3.0};
    std::cout << "Point: (" << p.x << ", " << p.y << ", " << p.z << ")\n";
}

// C++20 ranges (basic example) - simplified version
void demonstrate_ranges() {
    std::cout << "\n=== Modern Range-Based Loops ===\n";
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Range-based for loop with index simulation
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << "Index " << i << ": " << numbers[i] << "\n";
    }
}

} // namespace fundamentals

int main() {
    std::cout << "=== C++ Syntax Basics - Modern Features ===\n";
    
    fundamentals::demonstrate_structured_bindings();
    fundamentals::process_value(42);
    fundamentals::process_value(3.14);
    fundamentals::process_value("hello");
    
#if __cpp_concepts >= 201907L
    fundamentals::concept_demo(100);
#endif
    
    fundamentals::demonstrate_optional();
    fundamentals::demonstrate_variant();
    fundamentals::demonstrate_designated_init();
    fundamentals::demonstrate_ranges();
    
    std::cout << "\nPI constant: " << fundamentals::PI << "\n";
    
    return 0;
}