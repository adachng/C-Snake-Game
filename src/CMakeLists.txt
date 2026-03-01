set(MAIN_TARGET snake_game)

add_subdirectory(snake_game_manager)

add_executable(${MAIN_TARGET}
    WIN32
    MACOSX_BUNDLE
    main.c
)

target_include_directories(${MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${MAIN_TARGET}
    PRIVATE SDL3::SDL3
    PRIVATE snake_game_manager
)
