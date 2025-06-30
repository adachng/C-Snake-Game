#include <stdlib.h>
#include <assert.h>

#include <rktest/rktest.h>
#include <snake_game_manager.h>

TEST(snake_gameplay_tests, scene_init_test)
{
    struct SnakeGameManager *snake_game_manager;
    char *scene_str;
    {
        // 3-by-3 map; expect:
        // ". . .\n"
        // ". @ .\n"
        // "$ . ."
        snake_game_manager = SnakeGameManager__init(3, 3);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);

        // printf("DEBUG: scene_string =\n%s\n\n", scene_str); // NOTE: toggle to see the game scene
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                "$ . .");

        SnakeGameManager__deinit(snake_game_manager);
        free(scene_str);
    }

    {
        // 4-by-4 map; expect:
        // ". . . .\n"
        // ". . @ .\n"
        // "$ . . .\n"
        // ". . . ."
        snake_game_manager = SnakeGameManager__init(4, 4);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);

        // printf("DEBUG: scene_string =\n%s\n\n", scene_str); // NOTE: toggle to see the game scene
        EXPECT_STREQ(scene_str, ". . . .\n"
                                ". . @ .\n"
                                "$ . . .\n"
                                ". . . .");

        SnakeGameManager__deinit(snake_game_manager);
        free(scene_str);
    }

    {
        // 5-by-5 map; expect:
        // ". . . . .\n"
        // ". . . . .\n"
        // ". . @ . .\n"
        // "$ . . . .\n"
        // ". . . . ."
        snake_game_manager = SnakeGameManager__init(5, 5);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);

        // printf("DEBUG: scene_string =\n%s\n\n", scene_str); // NOTE: toggle to see the game scene
        EXPECT_STREQ(scene_str,
                     ". . . . .\n"
                     ". . . . .\n"
                     ". . @ . .\n"
                     "$ . . . .\n"
                     ". . . . .");

        SnakeGameManager__deinit(snake_game_manager);
        free(scene_str);
    }

    {
        // 5-by-4 map; expect:
        // ". . . . .\n"
        // ". . @ . .\n"
        // "$ . . . .\n"
        // ". . . . ."
        snake_game_manager = SnakeGameManager__init(5, 4);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);

        // printf("DEBUG: scene_string =\n%s\n\n", scene_str); // NOTE: toggle to see the game scene
        EXPECT_STREQ(scene_str, ". . . . .\n"
                                ". . @ . .\n"
                                "$ . . . .\n"
                                ". . . . .");

        SnakeGameManager__deinit(snake_game_manager);
        free(scene_str);
    }

    {
        // 4-by-5 map; expect:
        // ". . . .\n"
        // ". . . .\n"
        // ". . @ .\n"
        // "$ . . .\n"
        // ". . . ."
        snake_game_manager = SnakeGameManager__init(4, 5);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);

        // printf("DEBUG: scene_string =\n%s\n\n", scene_str); // NOTE: toggle to see the game scene
        EXPECT_STREQ(scene_str, ". . . .\n"
                                ". . . .\n"
                                ". . @ .\n"
                                "$ . . .\n"
                                ". . . .");

        SnakeGameManager__deinit(snake_game_manager);
        free(scene_str);
    }
}

TEST(snake_gameplay_tests, coordinates_test)
{
    struct SnakeGameManager *snake_game_manager;
    int snake_head_x, snake_head_y;

    // ". . .\n"
    // ". @ .\n"
    // "$ . ."
    snake_game_manager = SnakeGameManager__init(3, 3);
    SnakeGameManager__debug_get_snake_head_coord(snake_game_manager, &snake_head_x, &snake_head_y);

    ASSERT_EQ(snake_head_x, 0);
    ASSERT_EQ(snake_head_y, 2);

    // ". . . .\n"
    // ". . @ .\n"
    // "$ . . .\n"
    // ". . . ."
    SnakeGameManager__deinit(snake_game_manager);
    snake_game_manager = SnakeGameManager__init(4, 4);
    SnakeGameManager__debug_get_snake_head_coord(snake_game_manager, &snake_head_x, &snake_head_y);

    ASSERT_EQ(snake_head_x, 0);
    ASSERT_EQ(snake_head_y, 2);

    // ". . . .\n"
    // ". . . .\n"
    // ". . @ .\n"
    // "$ . . .\n"
    // ". . . .\n"
    // ". . . ."
    SnakeGameManager__deinit(snake_game_manager);
    snake_game_manager = SnakeGameManager__init(4, 6);
    SnakeGameManager__debug_get_snake_head_coord(snake_game_manager, &snake_head_x, &snake_head_y);

    ASSERT_EQ(snake_head_x, 0);
    ASSERT_EQ(snake_head_y, 3);
}

TEST(snake_gameplay_tests, basic_movement_test)
{
    struct SnakeGameManager *snake_game_manager;
    char *scene_str;

    {
        // ". . .\n"
        // ". @ .\n"
        // "$ . ."
        snake_game_manager = SnakeGameManager__init(3, 3);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);

        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                "$ . .");
        free(scene_str);
    }

    {
        // ". . .\n"
        // ". @ .\n"
        // ". $ ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                ". $ .");
        free(scene_str);
    }

    {
        // ". . .\n"
        // ". @ .\n"
        // ". . $"
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                ". . $");
        free(scene_str);
    }

    SnakeGameManager__set_direction(snake_game_manager, 'w');
    {
        // ". . .\n"
        // ". @ $\n"
        // ". . ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ $\n"
                                ". . .");
        free(scene_str);
    }

    {
        // ". . $\n"
        // ". @ .\n"
        // ". . ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . $\n"
                                ". @ .\n"
                                ". . .");
        free(scene_str);
    }

    SnakeGameManager__set_direction(snake_game_manager, 'a');
    {
        // ". $ .\n"
        // ". @ .\n"
        // ". . ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". $ .\n"
                                ". @ .\n"
                                ". . .");
        free(scene_str);
    }

    {
        // "$ . .\n"
        // ". @ .\n"
        // ". . ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, "$ . .\n"
                                ". @ .\n"
                                ". . .");
        free(scene_str);
    }

    SnakeGameManager__set_direction(snake_game_manager, 's');
    {
        // ". . .\n"
        // "$ @ .\n"
        // ". . ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                "$ @ .\n"
                                ". . .");
        free(scene_str);
    }

    {
        // ". . .\n"
        // ". @ .\n"
        // "$ . ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                "$ . .");
        free(scene_str);
    }

    SnakeGameManager__set_direction(snake_game_manager, 'd');
    {
        // ". . .\n"
        // ". @ .\n"
        // ". $ ."
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                ". $ .");
        free(scene_str);
    }

    {
        // ". . .\n"
        // ". @ .\n"
        // ". . $"
        SnakeGameManager__update(snake_game_manager);

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                ". . $");
        free(scene_str);
    }

    SnakeGameManager__deinit(snake_game_manager);
}

TEST(snake_gameplay_tests, game_score_and_movement_test)
{
    struct SnakeGameManager *snake_game_manager;
    char *scene_str;

    {
        // ". . .\n"
        // ". @ .\n"
        // "$ . ."
        snake_game_manager = SnakeGameManager__init(3, 3);

        EXPECT_EQ(SnakeGameManager__get_score(snake_game_manager), 0);

        SnakeGameManager__update(snake_game_manager);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". . .\n"
                                ". @ .\n"
                                ". $ .");
        free(scene_str);
        EXPECT_EQ(SnakeGameManager__get_score(snake_game_manager), 0);
    }

    {
        SnakeGameManager__set_direction(snake_game_manager, 'w');
        SnakeGameManager__update(snake_game_manager);
        EXPECT_EQ(SnakeGameManager__get_score(snake_game_manager), 1);

        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 1, 0);
        // At this point:
        // ". @ .\n"
        // ". $ .\n"
        // ". # ."
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, ". @ .\n"
                                ". $ .\n"
                                ". # .");
        free(scene_str);
    }

    {
        SnakeGameManager__update(snake_game_manager);
        EXPECT_EQ(SnakeGameManager__get_score(snake_game_manager), 2);

        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 0);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, "@ $ .\n"
                                ". # .\n"
                                ". # .");
        free(scene_str);
    }

    {
        SnakeGameManager__set_direction(snake_game_manager, 'a');
        SnakeGameManager__update(snake_game_manager);
        EXPECT_EQ(SnakeGameManager__get_score(snake_game_manager), 3);

        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 1);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, "$ # .\n"
                                "@ # .\n"
                                ". # .");
        free(scene_str);
    }

    {
        SnakeGameManager__set_direction(snake_game_manager, 's');
        SnakeGameManager__update(snake_game_manager);
        EXPECT_EQ(SnakeGameManager__get_score(snake_game_manager), 4);

        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 2);
        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, "# # .\n"
                                "$ # .\n"
                                "@ # .");
        free(scene_str);
    }

    SnakeGameManager__deinit(snake_game_manager);
}

TEST(snake_gameplay_tests, game_lost_test)
{
    struct SnakeGameManager *snake_game_manager;

    {
        // Immediately turn left and collide with wall
        // ". . .\n"
        // ". @ .\n"
        // "$ . ."
        snake_game_manager = SnakeGameManager__init(3, 3);

        SnakeGameManager__set_direction(snake_game_manager, 'a');
        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__update(snake_game_manager);
        EXPECT_TRUE(SnakeGameManager__is_game_lost(snake_game_manager));
    }

    SnakeGameManager__deinit(snake_game_manager);

    {
        // Self-collision:
        // ". . .\n"
        // ". @ .\n"
        // "$ . ."
        snake_game_manager = SnakeGameManager__init(3, 3);

        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__update(snake_game_manager);
        // ". . .\n"
        // ". @ .\n"
        // ". $ ."
        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__set_direction(snake_game_manager, 'w');
        SnakeGameManager__update(snake_game_manager);
        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 1, 0);
        // ". @ .\n"
        // ". $ .\n"
        // ". # ."
        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__update(snake_game_manager);
        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 0);
        // "@ $ .\n"
        // ". # .\n"
        // ". # ."
        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__set_direction(snake_game_manager, 'a');
        SnakeGameManager__update(snake_game_manager);
        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 1);
        // "$ # .\n"
        // "@ # .\n"
        // ". # ."
        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__set_direction(snake_game_manager, 's');
        SnakeGameManager__update(snake_game_manager);
        SnakeGameManager__debug_set_apple_coord(snake_game_manager, 2, 2);

        char *scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, "# # .\n"
                                "$ # .\n"
                                ". # @");
        free(scene_str);

        EXPECT_FALSE(SnakeGameManager__is_game_lost(snake_game_manager));
        SnakeGameManager__set_direction(snake_game_manager, 'd');
        SnakeGameManager__update(snake_game_manager);
        EXPECT_TRUE(SnakeGameManager__is_game_lost(snake_game_manager));

        scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
        EXPECT_STREQ(scene_str, "# # .\n"
                                "# $# .\n" // direction irrelevant to order of $# (or #$)
                                ". . @");
        free(scene_str);
    }
}

TEST(snake_gameplay_tests, game_won_test)
{
    struct SnakeGameManager *snake_game_manager;

    snake_game_manager = SnakeGameManager__init(3, 3);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 1, 2);
    // ". . .\n"
    // ". . .\n"
    // "$ @ ."

    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 2, 2);
    // ". . .\n"
    // ". . .\n"
    // "# $ @"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 2, 1);
    // ". . .\n"
    // ". . @\n"
    // "# # $"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__set_direction(snake_game_manager, 'w');
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 2, 0);
    // ". . @\n"
    // ". . $\n"
    // "# # #"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 1, 0);
    // ". @ $\n"
    // ". . #\n"
    // "# # #"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__set_direction(snake_game_manager, 'a');
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 0);
    // "@ $ #\n"
    // ". . #\n"
    // "# # #"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 1);
    // "$ # #\n"
    // "@ . #\n"
    // "# # #"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__set_direction(snake_game_manager, 's');
    SnakeGameManager__update(snake_game_manager);
    // "# # #\n"
    // "$ @ #\n"
    // "# # #"
    EXPECT_FALSE(SnakeGameManager__is_game_won(snake_game_manager));
    SnakeGameManager__set_direction(snake_game_manager, 'd');
    SnakeGameManager__update(snake_game_manager);
    EXPECT_TRUE(SnakeGameManager__is_game_won(snake_game_manager));

    SnakeGameManager__deinit(snake_game_manager);
}

TEST(snake_gameplay_tests, invalid_direction_change_test)
{
    struct SnakeGameManager *snake_game_manager;
    char *scene_str;

    snake_game_manager = SnakeGameManager__init(3, 3);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 1, 2);
    // ". . .\n"
    // ". . .\n"
    // "$ @ ."

    SnakeGameManager__update(snake_game_manager);

    SnakeGameManager__set_direction(snake_game_manager, 'a');
    SnakeGameManager__update(snake_game_manager);
    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 0, 0);

    scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, "@ . .\n"
                            ". . .\n"
                            ". # $");
    free(scene_str);

    SnakeGameManager__set_direction(snake_game_manager, 'w');
    SnakeGameManager__update(snake_game_manager);
    // "@ . .\n"
    // ". . $\n"
    // ". . #"
    SnakeGameManager__set_direction(snake_game_manager, 's');
    SnakeGameManager__update(snake_game_manager);
    scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, "@ . $\n"
                            ". . #\n"
                            ". . .");
    free(scene_str);

    SnakeGameManager__debug_set_apple_coord(snake_game_manager, 2, 2);
    SnakeGameManager__set_direction(snake_game_manager, 'a');
    SnakeGameManager__update(snake_game_manager);
    // ". $ #\n"
    // ". . .\n"
    // ". . @"
    SnakeGameManager__set_direction(snake_game_manager, 'd');
    SnakeGameManager__update(snake_game_manager);
    scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, "$ # .\n"
                            ". . .\n"
                            ". . @");
    free(scene_str);

    SnakeGameManager__set_direction(snake_game_manager, 's');
    SnakeGameManager__update(snake_game_manager);
    // "# . .\n"
    // "$ . .\n"
    // ". . @"
    SnakeGameManager__set_direction(snake_game_manager, 'w');
    SnakeGameManager__update(snake_game_manager);
    scene_str = SnakeGameManager_alloc_render_scene_to_string(snake_game_manager);
    EXPECT_STREQ(scene_str, ". . .\n"
                            "# . .\n"
                            "$ . @");
    free(scene_str);
}
