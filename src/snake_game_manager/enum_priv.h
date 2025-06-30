#pragma once
#ifndef SRC_SNAKE_GAME_MANAGER_ENUM_PRIV_H
#define SRC_SNAKE_GAME_MANAGER_ENUM_PRIV_H

enum GridCellState
{
    CELL_EMPTY = 0U,
    CELL_APPLE = 1U << 0U,
    CELL_SNAKE_HEAD = 1U << 1U,
    CELL_SNAKE_BODY = 1U << 2U,
}; // enum SceneGridPointState

enum SnakeDirection
{
    DIRECTION_UP = 'w',
    DIRECTION_LEFT = 'a',
    DIRECTION_DOWN = 's',
    DIRECTION_RIGHT = 'd',
}; // enum SnakeDirection

#endif // SRC_SNAKE_GAME_MANAGER_ENUM_PRIV_H
