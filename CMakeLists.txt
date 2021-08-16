cmake_minimum_required(VERSION 3.16)
project(comoverip)

set(CMAKE_CXX_STANDARD 20)

add_compile_options( "-Wall" )
add_compile_options( "-Werror" )
add_compile_options( "-Wextra" )
add_compile_options( "-Wpedantic" )

set(INCLUDES
          comoverip
          contrib/asio/asio/include
)

set(SOURCES_EXCHANGE
          ${PROJECT_SOURCE_DIR}/comoverip/exchange/exchange.cpp
)

set(SOURCES_COMMON
          ${PROJECT_SOURCE_DIR}/comoverip/common/logger.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/common/data.cpp
)

set(SOURCES
          ${SOURCES_EXCHANGE}
          ${SOURCES_COMMON}
          comoverip/message_data.cpp
          comoverip/rtu_client.cpp
          comoverip/udp_client.cpp
)

include_directories(${INCLUDES})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(
          ${PROJECT_NAME}
          pthread
)

add_subdirectory(contrib/googletest)
add_subdirectory(test)