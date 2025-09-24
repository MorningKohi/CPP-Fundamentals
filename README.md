# C++ Fundamentals - Modern Learning Environment

A comprehensive C++ learning laboratory featuring modern C++17/20/23 standards, practical examples, memory management, STL containers, algorithms, template metaprogramming, and concurrent programming patterns.

## 🚀 Quick Start

### Prerequisites
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.16+ (optional, for CMake builds)
- Google Test (automatically downloaded via CMake)

### Building with Makefile (Recommended for quick start)

```bash
# Build all examples
make all

# Build individual modules
make syntax          # C++ syntax basics
make types           # Data types and type deduction
make control         # Control flow structures  
make functions       # Functions and lambdas
make classes         # Classes and OOP
make memory          # Memory management examples

# Clean build artifacts
make clean

# Show available targets
make install
```

### Building with CMake

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run tests
ctest
```

## 📚 Learning Modules

### 1. C++ Fundamentals (`src/01_fundamentals/`)

**Modern C++17/20/23 Features:**
- ✅ Structured bindings (C++17)
- ✅ `std::optional` and `std::variant` (C++17) 
- ✅ Template argument deduction
- ✅ `if constexpr` and concepts (C++20)
- ✅ Designated initializers
- ✅ Range-based for loops

**Run Examples:**
```bash
./bin/syntax_basics     # Modern syntax features
./bin/data_types        # Type system and deduction  
./bin/control_flow      # Control structures
./bin/functions_demo    # Functions, lambdas, higher-order functions
./bin/classes_demo      # OOP, RAII, rule of 3/5
```

### 2. Memory Management (`src/02_memory_management/`)

**Comprehensive Memory Management:**
- ✅ Stack vs Heap allocation
- ✅ Smart pointers (`unique_ptr`, `shared_ptr`, `weak_ptr`)
- ✅ Memory leak detection and prevention
- ✅ Custom allocators and memory pools
- ✅ RAII patterns
- ✅ Exception safety

**Run Examples:**
```bash
./bin/stack_heap        # Memory layout and allocation
./bin/smart_pointers    # Modern memory management
./bin/memory_leaks      # Leak prevention techniques
./bin/custom_allocators # Memory pools and custom allocation
```

### 3. STL Containers (Coming Soon)
- Vector, List, Map implementations
- Container performance comparisons
- Custom containers

### 4. Algorithms (Coming Soon)  
- Sorting and searching algorithms
- STL algorithm usage
- Custom algorithm implementations

### 5. Template Metaprogramming (Coming Soon)
- Function and class templates
- Template specialization
- Variadic templates
- SFINAE and concepts

### 6. Concurrent Programming (Coming Soon)
- Thread basics and synchronization
- Mutex and atomic operations
- Async programming and futures
- Thread-safe data structures

## 🧪 Testing

### Unit Tests
The project includes comprehensive unit tests using Google Test:

```bash
# Build and run all tests
make test

# Or with CMake
cd build && ctest --verbose
```

**Test Coverage:**
- ✅ Fundamental C++ features
- ✅ Memory management
- ✅ STL container operations
- ✅ Algorithm correctness  
- ✅ Template functionality
- ✅ Concurrency primitives

### Manual Testing
Each module can be run independently to see live demonstrations:

```bash
# Interactive examples with output
./bin/syntax_basics
./bin/smart_pointers  
# ... etc
```

## 🔧 Development Setup

### VS Code Configuration
Debug configurations are provided in `.vscode/`:

- **Launch individual examples** - Debug specific modules
- **Attach to running process** - Debug live applications  
- **Unit test debugging** - Step through test cases

### Debugging Features
- ✅ GDB/LLDB integration
- ✅ Memory debugging with Valgrind support
- ✅ Address Sanitizer configuration
- ✅ Custom debug allocators with leak tracking

## 📁 Project Structure

```
CPP-Fundamentals/
├── src/                          # Source code modules
│   ├── 01_fundamentals/         # C++ basics and modern features
│   ├── 02_memory_management/    # Memory management examples  
│   ├── 03_stl_containers/       # STL and custom containers
│   ├── 04_algorithms/           # Algorithm implementations
│   ├── 05_templates/            # Template metaprogramming
│   └── 06_concurrency/          # Concurrent programming
├── tests/                        # Unit tests
├── examples/                     # Additional example programs
├── utils/                        # Utility headers and tools
├── docs/                         # Documentation
├── build/                        # Build artifacts (CMake)
├── bin/                          # Compiled executables (Makefile)
├── CMakeLists.txt               # CMake configuration
├── Makefile                     # Make configuration  
└── README.md                    # This file
```

## 🎯 Learning Path

### Beginner (Start Here)
1. **C++ Syntax Basics** - Modern language features
2. **Data Types** - Type system and auto deduction
3. **Control Flow** - Loops, conditionals, exceptions
4. **Functions** - Functions, lambdas, higher-order programming

### Intermediate  
5. **Classes and OOP** - Object-oriented programming
6. **Memory Management** - Smart pointers and RAII
7. **STL Containers** - Standard library containers
8. **Algorithms** - Standard and custom algorithms

### Advanced
9. **Template Metaprogramming** - Generic programming
10. **Concurrent Programming** - Multi-threading and async
11. **Custom Allocators** - Memory pool implementations
12. **Performance Optimization** - Profiling and optimization

## 💡 Key Learning Objectives

### Modern C++ Best Practices
- ✅ RAII (Resource Acquisition Is Initialization)  
- ✅ Rule of Three/Five/Zero
- ✅ Prefer smart pointers over raw pointers
- ✅ Use `const` correctness
- ✅ Exception safety guarantees
- ✅ Move semantics optimization

### Memory Management Mastery
- ✅ Understanding stack vs heap allocation
- ✅ Automatic memory management with smart pointers
- ✅ Custom memory allocation strategies
- ✅ Memory leak detection and prevention
- ✅ Cache-friendly data structures

### Template Programming
- ✅ Generic programming techniques
- ✅ Template specialization patterns
- ✅ SFINAE and concept constraints
- ✅ Variadic template programming
- ✅ Metaprogramming with type traits

### Concurrent Programming
- ✅ Thread safety and synchronization
- ✅ Lock-free programming with atomics
- ✅ Async programming patterns
- ✅ Parallel algorithm execution

## 🛠 Build System Features

### Makefile Targets
- `make all` - Build all examples
- `make syntax` - C++ syntax examples
- `make memory` - Memory management examples  
- `make test` - Build and run unit tests
- `make clean` - Clean build artifacts
- `make install` - Show usage information

### CMake Features  
- Automatic dependency management
- Google Test integration
- Cross-platform compatibility
- Debug/Release configurations
- Static analysis integration

## 🔍 Debugging and Profiling

### Memory Debugging
```bash
# Run with AddressSanitizer
g++ -fsanitize=address -g -o program program.cpp
./program

# Run with Valgrind
valgrind --leak-check=full ./program
```

### Performance Profiling
```bash
# Compile with profiling
g++ -pg -O2 -o program program.cpp
./program
gprof program gmon.out > analysis.txt
```

## 📖 Additional Resources

### Recommended Reading
- "Effective Modern C++" by Scott Meyers
- "C++ Core Guidelines" by Bjarne Stroustrup
- "A Tour of C++" by Bjarne Stroustrup

### Online Resources
- [C++ Reference](https://cppreference.com)
- [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines)
- [Compiler Explorer](https://godbolt.org) for assembly inspection

## 🤝 Contributing

Contributions are welcome! Please:
1. Follow modern C++ best practices
2. Add comprehensive unit tests
3. Include documentation and examples
4. Ensure cross-platform compatibility

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

---

**Happy Learning! 🚀**

Start your C++ journey with `make syntax` and work your way through each module. Each example is designed to build upon previous concepts while introducing new modern C++ features and best practices.
