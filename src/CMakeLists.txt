add_library(tpool SHARED threadpool.cc)
target_include_directories(tpool PUBLIC "${PROJECT_SOURCE_DIR}/include")
