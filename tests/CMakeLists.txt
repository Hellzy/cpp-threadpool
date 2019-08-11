add_executable(test_basic basic.cc)
target_link_libraries(test_basic PUBLIC tpool)
add_test(NAME basic COMMAND test_basic 1 2 3)
set_tests_properties (basic PROPERTIES PASS_REGULAR_EXPRESSION "1 2 3\n")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_basic)
