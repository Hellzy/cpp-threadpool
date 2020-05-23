add_executable(test_basic_no_fut basic_no_fut.cc)
target_link_libraries(test_basic_no_fut PUBLIC tpool)
add_test(NAME basic_no_fut COMMAND test_basic_no_fut 1 2 3)
set_tests_properties (basic_no_fut PROPERTIES PASS_REGULAR_EXPRESSION "1 2 3\n")

add_executable(test_basic_fut basic_fut.cc)
target_link_libraries(test_basic_fut PUBLIC tpool)
add_test(NAME basic_fut COMMAND test_basic_fut)

add_executable(test_multiple multiple.cc)
target_link_libraries(test_multiple PUBLIC tpool)
add_test(NAME multiple COMMAND test_multiple)

add_executable(test_message_queue message_queue.cc)
target_link_libraries(test_message_queue PUBLIC tpool)
add_test(NAME message_queue COMMAND test_message_queue)
set_tests_properties (message_queue PROPERTIES PASS_REGULAR_EXPRESSION "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n")

add_executable(test_threaded_message_queue threaded_message_queue.cc)
target_link_libraries(test_threaded_message_queue PUBLIC tpool)
add_test(NAME threaded_message_queue COMMAND test_threaded_message_queue)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_basic_no_fut
                                                               test_basic_fut
                                                               test_multiple
                                                               test_message_queue
                                                               test_threaded_message_queue)
