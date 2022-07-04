project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})


include_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../proto/biginteger.proto ../proto/finite_field.proto)

# adding the Google_Tests_run target
add_executable(Google_Tests_run kmp_search_test.cpp biginteger_arithmetic_test.cpp ${PROTO_HDRS})

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run common_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)