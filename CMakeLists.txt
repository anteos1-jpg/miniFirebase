cmake_minimum_required(VERSION 3.16)
project(mini_firebase)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Directory containing your proto files
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PROTO_FILES "${PROTO_DIR}/*.proto")

# Lists to store generated files
set(GENERATED_SRCS)
set(GENERATED_HDRS)

foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

    set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc")
    set(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.h")
    set(GRPC_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.h")

    list(APPEND GENERATED_SRCS ${PROTO_SRC} ${GRPC_SRC})
    list(APPEND GENERATED_HDRS ${PROTO_HDR} ${GRPC_HDR})

    add_custom_command(
        OUTPUT ${PROTO_SRC} ${PROTO_HDR} ${GRPC_SRC} ${GRPC_HDR}
        COMMAND protoc
        ARGS --proto_path=${PROTO_DIR}
             --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
             --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
             --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`
             ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
    )
endforeach()

# --- Server executable ---
add_executable(server
    server.cpp
    ${GENERATED_SRCS}
)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(server
    gRPC::grpc++
    protobuf::libprotobuf
)

# --- Client executable ---
add_executable(client
    client.cpp
    ${GENERATED_SRCS}
)

target_include_directories(client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(client
    gRPC::grpc++
    protobuf::libprotobuf
)