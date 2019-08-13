include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    add_definitions(-DTHREADS_NB=${N})
endif()


add_library(tpool SHARED threadpool.cc worker.cc work_item_scheduler.cc)
target_include_directories(tpool PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(tpool pthread)
