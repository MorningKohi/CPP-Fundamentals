#include <gtest/gtest.h>
#include <memory>

// Test smart pointer functionality
TEST(MemoryTest, UniquePtr) {
    auto ptr = std::make_unique<int>(42);
    
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(*ptr, 42);
    
    auto moved_ptr = std::move(ptr);
    EXPECT_TRUE(ptr == nullptr);
    EXPECT_TRUE(moved_ptr != nullptr);
    EXPECT_EQ(*moved_ptr, 42);
}

TEST(MemoryTest, SharedPtr) {
    auto shared1 = std::make_shared<int>(100);
    EXPECT_EQ(shared1.use_count(), 1);
    
    {
        auto shared2 = shared1;
        EXPECT_EQ(shared1.use_count(), 2);
        EXPECT_EQ(shared2.use_count(), 2);
        EXPECT_EQ(*shared2, 100);
    }
    
    EXPECT_EQ(shared1.use_count(), 1);
}

TEST(MemoryTest, WeakPtr) {
    std::weak_ptr<int> weak_ptr;
    
    {
        auto shared = std::make_shared<int>(200);
        weak_ptr = shared;
        
        EXPECT_FALSE(weak_ptr.expired());
        
        if (auto locked = weak_ptr.lock()) {
            EXPECT_EQ(*locked, 200);
        } else {
            FAIL() << "Should be able to lock weak_ptr";
        }
    }
    
    EXPECT_TRUE(weak_ptr.expired());
    EXPECT_TRUE(weak_ptr.lock() == nullptr);
}