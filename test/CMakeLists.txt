enable_testing()

set(TEST_TARGET test)

set(SOURCES_TEST
          ${SOURCES_EXCHANGE}
          exchange_test.cpp
)

add_executable(${TEST_TARGET} ${SOURCES_TEST})

target_link_libraries(
          ${TEST_TARGET}
          gtest
          gtest_main
)