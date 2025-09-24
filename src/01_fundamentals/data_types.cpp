/**
 * C++ Data Types - Comprehensive Modern Examples
 * Covers built-in types, type deduction, and modern type utilities
 */

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>
#include <vector>
#include <array>
#include <string>

namespace datatypes {

// Type deduction utilities
template<typename T>
std::string get_type_name() {
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
        std::free
    };
    return (status == 0) ? res.get() : typeid(T).name();
}

// Built-in types demonstration
void demonstrate_builtin_types() {
    std::cout << "\n=== Built-in Data Types ===\n";
    
    // Integer types
    int8_t tiny = -128;
    uint8_t utiny = 255;
    int16_t small = -32768;
    uint16_t usmall = 65535;
    int32_t medium = -2147483648;
    uint32_t umedium = 4294967295U;
    int64_t large = (-9223372036854775807LL - 1); // Proper way to express most negative value
    uint64_t ularge = 18446744073709551615ULL;
    
    std::cout << "int8_t: " << static_cast<int>(tiny) << " (size: " << sizeof(tiny) << ")\n";
    std::cout << "uint8_t: " << static_cast<unsigned>(utiny) << " (size: " << sizeof(utiny) << ")\n";
    std::cout << "int16_t: " << small << " (size: " << sizeof(small) << ")\n";
    std::cout << "uint16_t: " << usmall << " (size: " << sizeof(usmall) << ")\n";
    std::cout << "int32_t: " << medium << " (size: " << sizeof(medium) << ")\n";
    std::cout << "uint32_t: " << umedium << " (size: " << sizeof(umedium) << ")\n";
    std::cout << "int64_t: " << large << " (size: " << sizeof(large) << ")\n";
    std::cout << "uint64_t: " << ularge << " (size: " << sizeof(ularge) << ")\n";
    
    // Floating point types
    float f = 3.14159f;
    double d = 2.718281828459045;
    long double ld = 1.4142135623730951L;
    
    std::cout << "\nfloat: " << f << " (size: " << sizeof(f) << ")\n";
    std::cout << "double: " << d << " (size: " << sizeof(d) << ")\n";
    std::cout << "long double: " << ld << " (size: " << sizeof(ld) << ")\n";
    
    // Character types
    char c = 'A';
    wchar_t wc = L'B';
    char16_t c16 = u'C';
    char32_t c32 = U'D';
    
    std::cout << "\nchar: " << c << " (size: " << sizeof(c) << ")\n";
    std::cout << "wchar_t size: " << sizeof(wc) << "\n";
    std::cout << "char16_t size: " << sizeof(c16) << "\n";
    std::cout << "char32_t size: " << sizeof(c32) << "\n";
    
    // Boolean
    bool flag = true;
    std::cout << "\nbool: " << std::boolalpha << flag << " (size: " << sizeof(flag) << ")\n";
}

// Auto type deduction
void demonstrate_auto() {
    std::cout << "\n=== Auto Type Deduction ===\n";
    
    auto integer = 42;                    // int
    auto floating = 3.14;                 // double
    auto character = 'x';                 // char
    auto text = "Hello";                  // const char*
    auto string = std::string("World");   // std::string
    
    std::cout << "auto integer = 42; -> " << get_type_name<decltype(integer)>() << "\n";
    std::cout << "auto floating = 3.14; -> " << get_type_name<decltype(floating)>() << "\n";
    std::cout << "auto character = 'x'; -> " << get_type_name<decltype(character)>() << "\n";
    std::cout << "auto text = \"Hello\"; -> " << get_type_name<decltype(text)>() << "\n";
    std::cout << "auto string = std::string(\"World\"); -> " << get_type_name<decltype(string)>() << "\n";
    
    // Auto with references and pointers
    int value = 100;
    auto& ref = value;              // int&
    auto* ptr = &value;             // int*
    const auto& cref = value;       // const int&
    
    std::cout << "\nauto& ref = value; -> " << get_type_name<decltype(ref)>() << "\n";
    std::cout << "auto* ptr = &value; -> " << get_type_name<decltype(ptr)>() << "\n";
    std::cout << "const auto& cref = value; -> " << get_type_name<decltype(cref)>() << "\n";
}

// Decltype examples
void demonstrate_decltype() {
    std::cout << "\n=== Decltype Usage ===\n";
    
    int x = 5;
    double y = 3.14;
    
    // decltype with expressions
    [[maybe_unused]] decltype(x) same_as_x = 10;        // int
    [[maybe_unused]] decltype(x + y) result_type = 0;   // double
    [[maybe_unused]] decltype((x)) ref_to_x = x;        // int& (note the extra parentheses)
    
    std::cout << "decltype(x) -> " << get_type_name<decltype(x)>() << "\n";
    std::cout << "decltype(x + y) -> " << get_type_name<decltype(x + y)>() << "\n";
    std::cout << "decltype((x)) -> " << get_type_name<decltype((x))>() << "\n";
    
    // Trailing return type
    auto add = [](auto a, auto b) -> decltype(a + b) {
        return a + b;
    };
    
    auto sum = add(5, 3.14);
    std::cout << "Lambda result: " << sum << " (type: " << get_type_name<decltype(sum)>() << ")\n";
}

// Type traits examples
void demonstrate_type_traits() {
    std::cout << "\n=== Type Traits ===\n";
    
    // Type checking
    std::cout << std::boolalpha;
    std::cout << "std::is_integral<int>: " << std::is_integral_v<int> << "\n";
    std::cout << "std::is_floating_point<double>: " << std::is_floating_point_v<double> << "\n";
    std::cout << "std::is_pointer<int*>: " << std::is_pointer_v<int*> << "\n";
    std::cout << "std::is_reference<int&>: " << std::is_reference_v<int&> << "\n";
    std::cout << "std::is_const<const int>: " << std::is_const_v<const int> << "\n";
    
    // Type transformations
    using IntPtr = std::add_pointer_t<int>;        // int*
    using IntRef = std::add_lvalue_reference_t<int>; // int&
    using ConstInt = std::add_const_t<int>;         // const int
    using RemovedConst = std::remove_const_t<const int>; // int
    
    std::cout << "\nType transformations:\n";
    std::cout << "std::add_pointer_t<int> -> " << get_type_name<IntPtr>() << "\n";
    std::cout << "std::add_lvalue_reference_t<int> -> " << get_type_name<IntRef>() << "\n";
    std::cout << "std::add_const_t<int> -> " << get_type_name<ConstInt>() << "\n";
    std::cout << "std::remove_const_t<const int> -> " << get_type_name<RemovedConst>() << "\n";
}

// Modern array types
void demonstrate_array_types() {
    std::cout << "\n=== Array Types ===\n";
    
    // C-style array
    int c_array[5] = {1, 2, 3, 4, 5};
    std::cout << "C-style array size: " << sizeof(c_array) / sizeof(c_array[0]) << "\n";
    
    // std::array (C++11)
    std::array<int, 5> std_array = {1, 2, 3, 4, 5};
    std::cout << "std::array size: " << std_array.size() << "\n";
    std::cout << "std::array type: " << get_type_name<decltype(std_array)>() << "\n";
    
    // std::vector (dynamic array)
    std::vector<int> vector = {1, 2, 3, 4, 5};
    std::cout << "std::vector size: " << vector.size() << "\n";
    std::cout << "std::vector capacity: " << vector.capacity() << "\n";
    std::cout << "std::vector type: " << get_type_name<decltype(vector)>() << "\n";
}

} // namespace datatypes

int main() {
    std::cout << "=== C++ Data Types Demonstration ===\n";
    
    datatypes::demonstrate_builtin_types();
    datatypes::demonstrate_auto();
    datatypes::demonstrate_decltype();
    datatypes::demonstrate_type_traits();
    datatypes::demonstrate_array_types();
    
    return 0;
}