cmake_minimum_required(VERSION 3.22)
project(ECC)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(abseil-cpp-master)
include_directories(abseil-cpp-master)
#find_package(Protobuf REQUIRED)
#include_directories(${PROTOBUF_INCLUDE_DIRS})


include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_subdirectory(common_lib)
include_directories(common_lib)

add_subdirectory(google-test)

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/biginteger.proto proto/finite_field.proto)
add_executable(ECC main.cpp)

target_link_libraries(ECC common_lib)
