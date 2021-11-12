cmake_minimum_required(VERSION 3.16)
project(comoverip)

set(CMAKE_CXX_STANDARD 17)

add_compile_options( "-Wall" )
add_compile_options( "-Werror" )
add_compile_options( "-Wextra" )
add_compile_options( "-Wpedantic" )

set(INCLUDES
          comoverip
          contrib/asio/asio/include
)

set(SOURCES
          ${PROJECT_SOURCE_DIR}/comoverip/exchange/exchange.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/common/logger.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/common/data.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/string_parser.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/source_args_parser.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/configurator.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/message_data.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/rtu_client.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/udp_client.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/tcp_client.cpp
          ${PROJECT_SOURCE_DIR}/comoverip/tcp_server.cpp
)

include_directories(${INCLUDES})

set(LIBS pthread)
IF(WIN32)
    set(LIBS ${LIBS} ws2_32 wsock32)
ENDIF()

add_executable(${PROJECT_NAME} comoverip/comoverip.cpp ${SOURCES})

target_link_libraries(
          ${PROJECT_NAME}
          ${LIBS}
)

add_subdirectory(contrib/googletest)
add_subdirectory(test)