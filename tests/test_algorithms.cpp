#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

// Test algorithm implementations
TEST(AlgorithmsTest, SortingAlgorithms) {
    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90};
    auto sorted_data = data;
    
    std::sort(sorted_data.begin(), sorted_data.end());
    
    EXPECT_TRUE(std::is_sorted(sorted_data.begin(), sorted_data.end()));
    EXPECT_EQ(sorted_data.front(), 11);
    EXPECT_EQ(sorted_data.back(), 90);
}

TEST(AlgorithmsTest, SearchAlgorithms) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Binary search (data must be sorted)
    bool found = std::binary_search(data.begin(), data.end(), 5);
    EXPECT_TRUE(found);
    
    found = std::binary_search(data.begin(), data.end(), 15);
    EXPECT_FALSE(found);
    
    // Linear search
    auto it = std::find(data.begin(), data.end(), 7);
    EXPECT_NE(it, data.end());
    EXPECT_EQ(*it, 7);
}