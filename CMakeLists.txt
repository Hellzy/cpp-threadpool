cmake_minimum_required (VERSION 3.0)
project (cpp-threadpool)

set (cpp-threadpool_VERSION_MAJOR 0)
set (cpp-threadpool_VERSION_MINOR 1)

include_directories("${PROJECT_BINARY_DIR}")

add_library(tpool SHARED src/threadpool.cc)
target_include_directories(tpool PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
