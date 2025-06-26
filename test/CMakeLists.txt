add_executable(tests
    test_main.c
    test_factorial.c
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE rktest)
