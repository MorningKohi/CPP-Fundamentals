#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>

// Test basic threading
TEST(ConcurrencyTest, BasicThread) {
    bool executed = false;
    
    std::thread t([&executed]() {
        executed = true;
    });
    
    t.join();
    EXPECT_TRUE(executed);
}

TEST(ConcurrencyTest, MutexProtection) {
    std::mutex mtx;
    int counter = 0;
    const int num_threads = 10;
    const int increments_per_thread = 1000;
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                std::lock_guard<std::mutex> lock(mtx);
                ++counter;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(counter, num_threads * increments_per_thread);
}

TEST(ConcurrencyTest, AtomicOperations) {
    std::atomic<int> atomic_counter{0};
    const int num_threads = 10;
    const int increments_per_thread = 1000;
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                atomic_counter++;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(atomic_counter.load(), num_threads * increments_per_thread);
}

TEST(ConcurrencyTest, Future) {
    auto future = std::async(std::launch::async, []() {
        return 42;
    });
    
    EXPECT_EQ(future.get(), 42);
}