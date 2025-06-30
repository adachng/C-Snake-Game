#pragma once
#ifndef SRC_SNAKE_GAME_MANAGER_SNAKE_PART_PRIV_H
#define SRC_SNAKE_GAME_MANAGER_SNAKE_PART_PRIV_H

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

struct SnakePart
{
    bool is_head;
    int coord_x;
    int coord_y;
    struct SnakePart *next;
}; // struct SnakePart

struct SnakePart *SnakePart__allocate_instance(const bool is_head,
                                               const int coord_x,
                                               const int coord_y,
                                               const struct SnakePart *next)
{
    struct SnakePart *ret = (struct SnakePart *)malloc(sizeof(struct SnakePart));
    assert(ret);

    ret->is_head = is_head;
    ret->coord_x = coord_x;
    ret->coord_y = coord_y;
    ret->next = next;

    return ret;
}

#endif // SRC_SNAKE_GAME_MANAGER_SNAKE_PART_PRIV_H
