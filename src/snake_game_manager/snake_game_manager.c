#include "snake_game_manager.h"

#include "snake_part_priv.h"
#include "enum_priv.h"

#include <assert.h>
#include <stdlib.h>

struct SnakeGameManager
{
    struct SnakePart *head;     // linked list, snake head; sets the scene (but does not handle apple)
    enum GridCellState **scene; // 2d array for snake game scene; only update via _update_scene()
    int rows;                   // y-axis; minimum 3, maximum 128 (arbitrary)
    int cols;                   // x-axis; minimum 3, maximum 128 (arbitrary)
    int apple_coord_x;
    int apple_coord_y;
    enum SnakeDirection current_direction;
};
#include "snake_game_manager_priv.h"

struct SnakeGameManager *SnakeGameManager__init(const int x, const int y)
{
    assert(x >= 3 && x <= 128);
    assert(y >= 3 && y <= 128);

    struct SnakeGameManager *ret = (struct SnakeGameManager *)malloc(sizeof(struct SnakeGameManager));
    ret->rows = y;
    ret->cols = x;

    ret->scene = (enum GridCellState **)malloc(sizeof(enum GridCellState *) * ret->rows);
    assert(ret->scene);

    for (int i = 0; i < ret->rows; i++)
    {
        ret->scene[i] = (enum GridCellState *)malloc(sizeof(enum GridCellState) * ret->cols);
        assert(ret->scene[i]);
    }

    // Initialise apple at middle of screen (prefer top-right) and snake head (leftmost 1 grid below apple).
    ret->apple_coord_x = ret->cols / 2;
    int snake_head_coord_y;
    if (ret->rows % 2)
    {
        ret->apple_coord_y = ret->rows / 2;
        snake_head_coord_y = ret->rows / 2 + 1; // start at leftmost just below apple
    }
    else
    {
        ret->apple_coord_y = ret->rows / 2 - 1;
        snake_head_coord_y = ret->rows / 2; // start at leftmost just below apple
    }
    ret->head = SnakePart__allocate_instance(true, 0, snake_head_coord_y, NULL);
    ret->current_direction = DIRECTION_RIGHT;

    _update_scene(ret);

    return ret;
}

void SnakeGameManager__deinit(struct SnakeGameManager *self)
{
    _debug_assert_pointers(self);

    for (int i = 0; i < self->rows; i++)
    {
        free(self->scene[i]);
    }
    free(self->scene);

    _deinit_snake_parts(self->head);

    free(self);
}

bool SnakeGameManager__is_game_won(struct SnakeGameManager *self)
{
    _debug_assert_pointers(self);

    bool ret = true;
    for (int i = 0; i < self->rows; i++)
    {
        for (int j = 0; j < self->cols; j++)
        {
            // If a cell is neither the head nor body, then game is not won yet.
            if (self->scene[i][j] != CELL_SNAKE_HEAD &&
                self->scene[i][j] != CELL_SNAKE_BODY)
                ret = false;
        }
    }
    return ret;
}

bool SnakeGameManager__is_game_lost(struct SnakeGameManager *self)
{
    _debug_assert_pointers(self);

    for (int i = 0; i < self->rows; i++)
    {
        for (int j = 0; j < self->cols; j++)
        {
            // If head collided with body, game is lost.
            if (self->scene[i][j] & CELL_SNAKE_HEAD &&
                self->scene[i][j] & CELL_SNAKE_BODY)
                return true;
        }
    }

    return false;
}

void SnakeGameManager__update(struct SnakeGameManager *self)
{
    _debug_assert_pointers(self);

    bool is_apple_eaten;
    switch (self->current_direction)
    {
    case DIRECTION_UP:
        _move_snake(self, 'w', &is_apple_eaten);
        break;
    case DIRECTION_DOWN:
        _move_snake(self, 's', &is_apple_eaten);
        break;
    case DIRECTION_LEFT:
        _move_snake(self, 'a', &is_apple_eaten);
        break;
    case DIRECTION_RIGHT:
        _move_snake(self, 'd', &is_apple_eaten);
        break;
    default:
        assert(false);
        break;
    }

    if (is_apple_eaten)
    {
        // WIP
    }

    _update_scene(self);
}

void SnakeGameManager__set_direction(struct SnakeGameManager *self, const char dir)
{
    _debug_assert_pointers(self);
    assert(dir == 'w' || dir == 'a' || dir == 's' || dir == 'd');

    switch (dir)
    {
    case 'w':
        self->current_direction = DIRECTION_UP;
        break;
    case 'a':
        self->current_direction = DIRECTION_LEFT;
        break;
    case 's':
        self->current_direction = DIRECTION_DOWN;
        break;
    default:
        self->current_direction = DIRECTION_RIGHT;
        break;
    }
}

char *SnakeGameManager_alloc_render_scene_to_string(struct SnakeGameManager *self)
{
    _debug_assert_pointers(self);

    int current_string_capacity = 64;
    int current_string_size = 0;
    char *ret = (char *)malloc(sizeof(char) * current_string_capacity);
    assert(ret);
    for (int i = 0; i < self->rows; i++) // start at topmost
    {
        for (int j = 0; j < self->cols; j++) // start at leftmost
        {
            if (self->scene[i][j] == CELL_EMPTY)
            {
                _append_char_to_dynamic_string(&ret, '.', &current_string_size, &current_string_capacity);
            }
            if (self->scene[i][j] & CELL_APPLE)
            {
                _append_char_to_dynamic_string(&ret, '@', &current_string_size, &current_string_capacity);
            }
            if (self->scene[i][j] & CELL_SNAKE_HEAD)
            {
                _append_char_to_dynamic_string(&ret, '$', &current_string_size, &current_string_capacity);
            }
            if (self->scene[i][j] & CELL_SNAKE_BODY)
            {
                _append_char_to_dynamic_string(&ret, '#', &current_string_size, &current_string_capacity);
            }

            if (j + 1 < self->cols)
                _append_char_to_dynamic_string(&ret, ' ', &current_string_size, &current_string_capacity);
        }
        if (i + 1 < self->rows)
            _append_char_to_dynamic_string(&ret, '\n', &current_string_size, &current_string_capacity);
    }
    return ret;
}

void SnakeGameManager__debug_get_snake_head_coord(struct SnakeGameManager *self,
                                                  int *x,
                                                  int *y)
{
    _debug_assert_pointers(self);

    *x = self->head->coord_x;
    *y = self->head->coord_y;
}
