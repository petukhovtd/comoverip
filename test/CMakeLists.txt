enable_testing()

set(TEST_TARGET runAllTest)

include_directories(
          ${PROJECT_SOURCE_DIR}/comoverip
          ${PROJECT_SOURCE_DIR}/contrib/googletest/googletest
)

set(
          SRC_TEST
          exchange_test.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/exchange/exchange.cpp
)

add_executable(${TEST_TARGET} ${SRC_TEST})

target_link_libraries(
          ${TEST_TARGET}
          gtest
          gtest_main
)