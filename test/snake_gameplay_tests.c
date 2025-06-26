#include <rktest/rktest.h>
#include <snake_game_manager.h>

TEST(snake_gameplay_tests, test1)
{
    EXPECT_TRUE(SnakeGameManager__func());
}

TEST(snake_gameplay_tests, test2)
{
    EXPECT_FALSE(!SnakeGameManager__func());
}
