enable_testing()

set(TEST_TARGET test)

set(SOURCES_TEST
          ${SOURCES}
          exchange_test.cpp
          string_parser_test.cpp
          source_arg_parser_test.cpp
          configurator_test.cpp
)

add_executable(${TEST_TARGET} ${SOURCES_TEST})

target_link_libraries(
          ${TEST_TARGET}
          ${LIBS}
          gtest
          gtest_main
)
