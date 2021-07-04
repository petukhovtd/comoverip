cmake_minimum_required(VERSION 3.16)
project(comoverip)

set(CMAKE_CXX_STANDARD 20)

add_compile_options( "-Wall" )
add_compile_options( "-Werror" )
add_compile_options( "-Wextra" )
add_compile_options( "-Wpedantic" )

set(INCLUDES
          comoverip
          contrib/asio-1.18.0/include
)

set(SOURCES_EXCHANGE
          ${PROJECT_SOURCE_DIR}/comoverip/exchange/exchange.cpp
)

set(SOURCES_COMMON
          ${PROJECT_SOURCE_DIR}/comoverip/common/logger.cpp
)

set(SOURCES
          ${SOURCES_EXCHANGE}
          ${SOURCES_COMMON}
          #comoverip/rtu_client.cpp
          #comoverip/message_data.cpp
)

include_directories(${INCLUDES})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

add_subdirectory(contrib/googletest)
add_subdirectory(test)