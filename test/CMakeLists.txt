add_executable(tests
    main.c
    factorial_tests.c
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE rktest)
