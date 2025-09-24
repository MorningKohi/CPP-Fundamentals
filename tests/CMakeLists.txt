# Tests CMakeLists.txt
add_executable(all_tests
    test_fundamentals.cpp
    test_memory.cpp
    test_stl.cpp
    test_algorithms.cpp
    test_templates.cpp
    test_concurrency.cpp
)

target_link_libraries(all_tests
    gtest_main
    gtest
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(all_tests)