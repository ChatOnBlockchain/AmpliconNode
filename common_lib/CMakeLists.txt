project(common_lib)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(../abseil-cpp-master)


include_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../proto/biginteger.proto ../proto/finite_field.proto ../proto/node.proto ../proto/nodes_list_server.proto)

set(HEADER_FILES
        kmp_search.h
        biginteger_arithmetic.h
        ${PROTO_HDRS}
        )

set(SOURCE_FILES
        kmp_search.cpp
        biginteger_arithmetic.cpp
        ${PROTO_SRCS}
        )


add_library(common_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(common_lib ${PROTOBUF_LIBRARY} absl::base absl::flat_hash_map absl::statusor)