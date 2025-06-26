add_library(snake_game_manager EXCLUDE_FROM_ALL
    snake_game_manager.c
)

target_include_directories(snake_game_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
