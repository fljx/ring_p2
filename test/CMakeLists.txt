enable_testing()

find_package(GTest REQUIRED)

add_executable(test_ring_p2 test_ring_p2.cpp)

target_link_libraries(test_ring_p2
 PRIVATE
  # gtest gtest_main # gtest
  # GTest::GTest
  gtest gtest_main
)

add_test(ring_gtests test_ring_p2)
