#include <gtest/gtest.h>
#include <type_traits>

// Template function for testing
template<typename T>
T add(T a, T b) {
    return a + b;
}

// Template class for testing
template<typename T>
class Container {
private:
    T value_;
public:
    explicit Container(T value) : value_(value) {}
    T get() const { return value_; }
    void set(T value) { value_ = value; }
};

// Test template functionality
TEST(TemplatesTest, FunctionTemplates) {
    EXPECT_EQ(add(5, 3), 8);
    EXPECT_DOUBLE_EQ(add(2.5, 1.5), 4.0);
    EXPECT_EQ(add(std::string("Hello"), std::string(" World")), "Hello World");
}

TEST(TemplatesTest, ClassTemplates) {
    Container<int> int_container(42);
    EXPECT_EQ(int_container.get(), 42);
    
    int_container.set(100);
    EXPECT_EQ(int_container.get(), 100);
    
    Container<std::string> string_container("Test");
    EXPECT_EQ(string_container.get(), "Test");
}

TEST(TemplatesTest, TypeTraits) {
    EXPECT_TRUE(std::is_integral_v<int>);
    EXPECT_FALSE(std::is_integral_v<double>);
    EXPECT_TRUE(std::is_floating_point_v<double>);
    EXPECT_FALSE(std::is_floating_point_v<int>);
    EXPECT_TRUE(std::is_pointer_v<int*>);
    EXPECT_FALSE(std::is_pointer_v<int>);
}