add_library(tpool SHARED threadpool.cc worker.cc work_item_scheduler.cc)
target_include_directories(tpool PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(tpool pthread)
