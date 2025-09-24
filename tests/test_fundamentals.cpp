#include <gtest/gtest.h>
#include <sstream>

namespace fundamentals {
    // Function declarations for testing
    void demonstrate_structured_bindings();
    void demonstrate_optional();
    void demonstrate_variant();
}

// Test fundamental C++ features
TEST(FundamentalsTest, StructuredBindings) {
    // Test that structured bindings work correctly
    auto pair = std::make_pair(42, "Hello");
    auto [number, text] = pair;
    
    EXPECT_EQ(number, 42);
    EXPECT_STREQ(text, "Hello");
}

TEST(FundamentalsTest, OptionalBasics) {
    std::optional<int> opt1 = 42;
    std::optional<int> opt2;
    
    EXPECT_TRUE(opt1.has_value());
    EXPECT_FALSE(opt2.has_value());
    
    EXPECT_EQ(*opt1, 42);
    EXPECT_EQ(opt1.value_or(0), 42);
    EXPECT_EQ(opt2.value_or(100), 100);
}

TEST(FundamentalsTest, VariantBasics) {
    std::variant<int, double, std::string> var;
    
    var = 42;
    EXPECT_EQ(std::get<int>(var), 42);
    EXPECT_EQ(var.index(), 0);
    
    var = 3.14;
    EXPECT_DOUBLE_EQ(std::get<double>(var), 3.14);
    EXPECT_EQ(var.index(), 1);
    
    var = std::string("Hello");
    EXPECT_EQ(std::get<std::string>(var), "Hello");
    EXPECT_EQ(var.index(), 2);
}