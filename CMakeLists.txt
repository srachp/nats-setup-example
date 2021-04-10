cmake_minimum_required(VERSION 3.0.0)
project(nats-c-client VERSION 0.1.0)

file ( GLOB NATS_SOURCES
    "*.h"
    "*.cpp"
)

#find_library(NATS_LIB NAMES libnats_static.a)

include_directories(
    "/<home_dir>/tools/nats.c/nats_compiled_gcc9/include"
    "."
)

link_directories(
    "/<home_dir>/tools/nats.c/nats_compiled_gcc9/lib64"
)

add_executable(nats ${NATS_SOURCES})

target_link_libraries(
    nats
    pthread
    nats_static
    ssl
    crypto
)