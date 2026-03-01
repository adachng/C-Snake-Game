#include <stdlib.h>

#include <rktest/rktest.h>
#include <snake_game_manager.h>

TEST(snake_gameplay_stress_tests, 20_by_20_win_test)
{
    struct SnakeGameManager *snake_game_manager;
    char *scene_str;

    snake_game_manager = SnakeGameManager__init(20, 20);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 1, 19);
    SnakeGameManager__debug_set_snake_head_coord(snake_game_manager, 0, 19);
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));

    scene_str = SnakeGameManager__alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            "$ @ . . . . . . . . . . . . . . . . . .");
    free(scene_str);

    SnakeGameManager__update(snake_game_manager);
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 2, 19);
    scene_str = SnakeGameManager__alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            "# $ @ . . . . . . . . . . . . . . . . .");
    free(scene_str);

    SnakeGameManager__update(snake_game_manager);
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 3, 19);
    scene_str = SnakeGameManager__alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            ". . . . . . . . . . . . . . . . . . . .\n"
                            "# # $ @ . . . . . . . . . . . . . . . .");
    free(scene_str);
}
