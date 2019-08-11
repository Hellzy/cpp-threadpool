cmake_minimum_required (VERSION 3.0)
project (cpp-threadpool LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set (cpp-threadpool_VERSION_MAJOR 0)
set (cpp-threadpool_VERSION_MINOR 1)

include_directories("${PROJECT_BINARY_DIR}")

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
