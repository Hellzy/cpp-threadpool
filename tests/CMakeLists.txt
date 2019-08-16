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


add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_basic_no_fut
                                                               test_basic_fut
                                                               test_multiple)
