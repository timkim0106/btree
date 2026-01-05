cmake_minimum_required(VERSION 3.15)
project(BTreeKVStore VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(BTREE_SOURCES
    src/btree/btree.cpp
    src/btree/node.cpp
)

set(STORAGE_SOURCES
    src/storage/page_manager.cpp
    src/storage/disk_manager.cpp
)

set(NETWORK_SOURCES
    src/network/server.cpp
    src/network/connection.cpp
)

set(UTILS_SOURCES
    src/utils/serializer.cpp
)

# Main executable
add_executable(kvstore
    src/main.cpp
    ${BTREE_SOURCES}
    ${STORAGE_SOURCES}
    ${NETWORK_SOURCES}
    ${UTILS_SOURCES}
)

# Link pthread for multi-threading
find_package(Threads REQUIRED)
target_link_libraries(kvstore PRIVATE Threads::Threads)

# Tests (optional, we'll add later)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
