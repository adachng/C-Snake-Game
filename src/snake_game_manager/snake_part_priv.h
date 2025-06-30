#pragma once
#ifndef SRC_SNAKE_GAME_MANAGER_SNAKE_PART_PRIV_H
#define SRC_SNAKE_GAME_MANAGER_SNAKE_PART_PRIV_H

#include <stdbool.h>

struct SnakePart
{
    bool is_head;
    int coord_x;
    int coord_y;
    struct SnakePart *next;
}; // struct SnakePart

// TODO: init head at(?)

#endif // SRC_SNAKE_GAME_MANAGER_SNAKE_PART_PRIV_H
