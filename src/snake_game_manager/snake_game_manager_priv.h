#pragma once
#ifndef SRC_SNAKE_GAME_MANAGER_UTIL_PRIV_H
#define SRC_SNAKE_GAME_MANAGER_UTIL_PRIV_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

static inline void _move_snake(struct SnakeGameManager *self, const char dir, bool *is_apple_eaten_p)
{
    assert(self->head);
    assert(dir == 'w' || dir == 'a' || dir == 's' || dir == 'd');

    switch (dir)
    {
    case 'w':
        self->head->coord_y = self->head->coord_y - 1;
        break;
    case 'a':
        self->head->coord_x = self->head->coord_x - 1;
        break;
    case 's':
        self->head->coord_y = self->head->coord_y + 1;
        break;
    case 'd':
        self->head->coord_x = self->head->coord_x + 1;
        break;
    }

    if (self->head->coord_x == self->apple_coord_x && self->head->coord_y == self->apple_coord_y)
        *is_apple_eaten_p = true;
    else
        *is_apple_eaten_p = false;

    // Create new part if apple is eaten
    if (*is_apple_eaten_p)
    {
        self->head->next = (struct SnakePart *)malloc(sizeof(struct SnakePart));
        self->head->next->is_head = false;
        self->head->next->next = NULL;
        self->head->next->coord_x = self->head->coord_x;
        self->head->next->coord_y = self->head->coord_y;
    }

    // Do "trailing"
    // WIP
}

static inline void _debug_assert_pointers(struct SnakeGameManager *ptr);
static inline void _update_scene(struct SnakeGameManager *self)
{
    _debug_assert_pointers(self);

    for (int i = 0; i < self->rows; i++)
    {
        for (int j = 0; j < self->cols; j++)
        {
            self->scene[i][j] = CELL_EMPTY;
        }
    }

    assert(self->apple_coord_y < self->rows && self->apple_coord_y >= 0);
    assert(self->apple_coord_x < self->cols && self->apple_coord_x >= 0);
    self->scene[self->apple_coord_y][self->apple_coord_x] = CELL_APPLE;
    assert(self->head);
    assert(self->head->coord_y < self->rows && self->head->coord_y >= 0);
    assert(self->head->coord_x < self->cols && self->head->coord_x >= 0);
    self->scene[self->head->coord_y][self->head->coord_x] = CELL_SNAKE_HEAD;

    struct SnakePart *current = self->head->next;
    while (current != NULL)
    {
        self->scene[current->coord_y][current->coord_x] = CELL_SNAKE_BODY;
    }
}

static inline void _deinit_snake_parts(struct SnakePart *head)
{
    assert(head);

    struct SnakePart *current = head;
    struct SnakePart *next = head->next;

    while (current != NULL)
    {
        free(current);
        current = next;
        if (current != NULL)
            next = current->next;
    }
}

static inline void _append_char_to_dynamic_string(char **str_p, const char c_to_append, int *current_size_p, int *current_capacity_p)
{
    assert(str_p);
    assert(*str_p);
    assert(current_size_p);
    assert(current_capacity_p);

    assert(*current_size_p < *current_capacity_p);

    // Double the capacity if at least half is used up.
    if (*current_size_p >= *current_capacity_p / 2)
    {
        *current_capacity_p = *current_capacity_p * 2;
        *str_p = (char *)realloc(*str_p, *current_capacity_p);
        assert(*str_p);
    }

    // Append the character before '\0'.
    (*str_p)[*current_size_p] = c_to_append;
    *current_size_p = *current_size_p + 1;

    // Double the capacity if at least half is used up.
    if (*current_size_p >= *current_capacity_p / 2)
    {
        *current_capacity_p = *current_capacity_p * 2;
        *str_p = (char *)realloc(*str_p, *current_capacity_p * sizeof(char));
        assert(*str_p);
    }

    (*str_p)[*current_size_p + 1] = '\0';
}

static inline void _debug_assert_pointers(struct SnakeGameManager *ptr)
{
    assert(ptr);
    assert(ptr->scene);
    assert(ptr->head);
}

#endif // SRC_SNAKE_GAME_MANAGER_UTIL_PRIV_H
