#pragma once
#ifndef SRC_SNAKE_GAME_MANAGER_SNAKE_GAME_MANAGER_H
#define SRC_SNAKE_GAME_MANAGER_SNAKE_GAME_MANAGER_H

#include <stdbool.h>

struct SnakeGameManager;
/**
 * @brief Constructor of SnakeGameManager (dynamically allocates).
 *
 * @param rows The y dimension of the map.
 * @param cols The x dimension of the map.
 * @return struct SnakeGameManager*
 */
struct SnakeGameManager *SnakeGameManager__init(const int rows, const int cols);
/**
 * @brief Destructor of SnakeGameManager. Frees the dynamically allocated instance.
 *
 * @param self The instance of SnakeGameManager that is freed.
 */
void SnakeGameManager__deinit(struct SnakeGameManager *self);

/**
 * @brief
 *
 * @param self SnakeGameManager instance.
 * @return true if the game is completed successfully.
 * @return false if the game is not yet completed.
 */
bool SnakeGameManager__is_game_won(struct SnakeGameManager *self);
/**
 * @brief
 *
 * @param self SnakeGameManager instance.
 * @return true if the game is failed by hitting the wall or the body (excluding the tail).
 * @return false if the game is not yet failed.
 */
bool SnakeGameManager__is_game_lost(struct SnakeGameManager *self);
int SnakeGameManager__get_score(struct SnakeGameManager *self);

/**
 * @brief Iterates the game tick once (i.e. moves the snake 1 grid towards its current direction).
 *
 * @param self SnakeGameManager instance.
 */
void SnakeGameManager__update(struct SnakeGameManager *self);

void SnakeGameManager__set_direction(struct SnakeGameManager *self, const char dir);

/**
 * @brief Get the dynamically-allocated string representation of the game scene.
 *
 * For example, for a 5-by-3 map with the snake having score of 1 with apple at right:
 * ". . . . .\n"
 * ". # $ . @\n"
 * ". . . . ."
 * , where # is body/tail, @ is apple, and $ is snake head
 *
 * @param self
 * @return Dymanically-allocated string.
 */
char *SnakeGameManager__alloc_render_scene_to_string(struct SnakeGameManager *self);

void SnakeGameManager__debug_get_snake_head_coord(struct SnakeGameManager *self,
                                                  int *x,
                                                  int *y);
void SnakeGameManager__debug_get_apple_coord(struct SnakeGameManager *self,
                                             int *x,
                                             int *y);
/**
 * @brief Sets the coordinates of the apple. Does not account for collisions.
 *
 * @param self SnakeGameManager instance.
 * @param x x-coordinate of the apple; 0 being leftmost.
 * @param y y-coordinate of the apple; 0 being topmost.
 */
void SnakeGameManager__debug_set_apple_coord(struct SnakeGameManager *self,
                                             const int x,
                                             const int y);
/**
 * @brief Sets the coordinates of the snake head. Does not account for collisions.
 *
 * @param self SnakeGameManager instance.
 * @param x x-coordinate of the snake head; 0 being leftmost.
 * @param y y-coordinate of the snake head; 0 being topmost.
 */
void SnakeGameManager__debug_set_snake_head_coord(struct SnakeGameManager *self,
                                                  const int x,
                                                  const int y);

#endif // SRC_SNAKE_GAME_MANAGER_SNAKE_GAME_MANAGER_H
