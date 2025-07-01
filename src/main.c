#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL3/SDL.h>

#include <snake_game_manager.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define APP_WINDOW_WIDTH 640      // default 640, must be > 0
#define APP_WINDOW_HEIGHT 480     // default 480, must be > 0
#define APP_GAME_MAP_MARGIN_PX 30 // default 30, must be >= 0

#define GAME_SNAKE_SPEED 2.0f     // default 2.0f, must be >= 0.0f
#define GAME_SPEED_UP_FACTOR 5.0f // default 5.0f
#define GAME_SCENE_WIDTH 20       // default 20
#define GAME_SCENE_HEIGHT 20      // default 20

#define GAME_TARGET_TICK_PERIOD_MS (1000.0f / GAME_SNAKE_SPEED)

typedef struct AppState
{
    Uint64 previous_tick;
    float tick_progression; // initially 0.0f, if >= 1.0f then -= 1.0f and update game
    struct SnakeGameManager *snake_game_manager;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_game_paused;      // if false, (delta time) * GAME_SNAKE_SPEED is added into tick progresion
    bool is_game_speeding_up; // if true, (delta time) * GAME_SNAKE_SPEED * GAME_SPEED_UP_FACTOR is added instead
} AppState;

static inline SDL_FRect get_centered_boundary(SDL_Window *window,
                                              const int h_margin,
                                              const int v_margin)
{
    SDL_assert(window != NULL);

    SDL_FRect ret;

    int window_width, window_height;
    if (!SDL_GetWindowSize(window, &window_width, &window_height))
    {
        printf("SDL_GetWindowSize error: %s\n", SDL_GetError());
        return ret;
    }

    if (window_width < window_height)
    {
        ret.w = (float)(window_width - 2 * h_margin);
    }
    else
    {
        ret.w = (float)(window_height - 2 * v_margin);
    }
    ret.h = ret.w;
    ret.x = (float)(window_width) / 2.0f - ret.w / 2.0f;
    ret.y = (float)(window_height - v_margin) - ret.h;
    return ret;
}

static inline bool render_map_border(SDL_Window *window,
                                     SDL_Renderer *renderer,
                                     const int h_margin,
                                     const int v_margin)
{
    SDL_assert(window != NULL);
    SDL_assert(renderer != NULL);

    if (!SDL_SetRenderDrawColor(renderer, 0U, 0U, 0U, SDL_ALPHA_OPAQUE))
    {
        printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
        return false;
    }
    if (!SDL_RenderClear(renderer))
    {
        printf("SDL_RenderClear error: %s\n", SDL_GetError());
        return false;
    }
    if (!SDL_SetRenderDrawColor(renderer, 255U, 255U, 255U, SDL_ALPHA_OPAQUE))
    {
        printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
        return false;
    }

    int window_width, window_height;
    if (!SDL_GetWindowSize(window, &window_width, &window_height))
    {
        printf("SDL_GetWindowSize error: %s\n", SDL_GetError());
        return false;
    }

    SDL_FRect map_boundary_box = get_centered_boundary(window, h_margin, v_margin);
    if (map_boundary_box.w <= 0.0f || map_boundary_box.h <= 0.0f)
        return false;

    if (!SDL_RenderRect(renderer, &map_boundary_box))
    {
        printf("SDL_RenderRect error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static inline bool render_gameplay_visuals(struct SnakeGameManager *snake_game_manager,
                                           SDL_Window *window,
                                           SDL_Renderer *renderer,
                                           const int h_margin,
                                           const int v_margin,
                                           const bool is_game_paused)
{
    SDL_assert(window != NULL);
    SDL_assert(renderer != NULL);

    if (!render_map_border(window, renderer, h_margin, v_margin))
    {
        return false;
    }

    enum GridCellState **scene = SnakeGameManager__get_scene_2d_array(snake_game_manager);
    int scene_width, scene_height;
    SnakeGameManager__get_dimensions(snake_game_manager, &scene_width, &scene_height);

    SDL_FRect map_boundary_box = get_centered_boundary(window, h_margin, v_margin);

    const float grid_px_height = (float)(map_boundary_box.h) / (float)(scene_height);
    for (int i = 0; i < scene_height; i++)
    {
        const float grid_px_width = (float)(map_boundary_box.w) / (float)(scene_width);
        for (int j = 0; j < scene_width; j++)
        {
            const Uint8 r = (scene[i][j] & CELL_APPLE) ? 255U : 0U;
            const Uint8 g = (scene[i][j] & CELL_SNAKE_BODY) ? 255U : 0U;
            const Uint8 b = (scene[i][j] & CELL_SNAKE_HEAD) ? 255U : 0U;
            const float xCoord = (float)(j)*grid_px_width + map_boundary_box.x;
            const float yCoord = (float)(i)*grid_px_height + map_boundary_box.y;
            SDL_FRect grid = {xCoord, yCoord, grid_px_width, grid_px_height};
            if (r > 0 || g > 0 || b > 0)
            {
                if (!SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE))
                {
                    printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
                    return false;
                }
                if (!SDL_RenderFillRect(renderer, &grid))
                {
                    printf("SDL_RenderRect error: %s\n", SDL_GetError());
                    return false;
                }
            }
        }
    }

    if (!SDL_SetRenderDrawColor(renderer, 255U, 255U, 255U, SDL_ALPHA_OPAQUE))
    {
        printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
        return false;
    }

    const float text_y = map_boundary_box.y + map_boundary_box.h + 10.0f;
    char text_content[256];
    if (is_game_paused)
    {
        if (!SDL_SetRenderDrawColor(renderer, 255U, 255U, 255U, SDL_ALPHA_OPAQUE))
        {
            printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
            return false;
        }
        strcpy(text_content, "Game paused. Press ESC to resume. Score: %d");
    }
    else if (SnakeGameManager__is_game_won(snake_game_manager))
    {
        if (!SDL_SetRenderDrawColor(renderer, 0U, 255U, 0U, SDL_ALPHA_OPAQUE))
        {
            printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
            return false;
        }
        strcpy(text_content, "Congratulations! You won! Press R to restart. Score: %d");
    }
    else if (SnakeGameManager__is_game_lost(snake_game_manager))
    {
        if (!SDL_SetRenderDrawColor(renderer, 255U, 0U, 0U, SDL_ALPHA_OPAQUE))
        {
            printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
            return false;
        }
        strcpy(text_content, "Game over! Press R to restart. Score: %d");
    }
    else
        strcpy(text_content, "Score: %d");

    if (!SDL_RenderDebugTextFormat(renderer,
                                   map_boundary_box.x,
                                   text_y,
                                   text_content,
                                   SnakeGameManager__get_score(snake_game_manager)))
    {
        printf("SDL_RenderDebugTextFormat error: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_RenderPresent(renderer))
    {
        printf("SDL_RenderPresent error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static inline void update_tick_progression(AppState *appstate_p, const Uint64 dt_ms)
{
    if (appstate_p->is_game_paused)
        return;

    float tick_increment;
    if (appstate_p->is_game_speeding_up)
        tick_increment = (float)dt_ms * GAME_SNAKE_SPEED * GAME_SPEED_UP_FACTOR / 1000.0f;
    else
        tick_increment = (float)dt_ms * GAME_SNAKE_SPEED / 1000.0f;

    appstate_p->tick_progression = appstate_p->tick_progression + tick_increment;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_SetAppMetadata("Snake Game CPP", "1.0.1", "com.github.adachng.SnakeGameCPP"))
    {
        return SDL_APP_FAILURE;
    }

    *appstate = (void *)malloc(sizeof(AppState));
    AppState *appstate_p = (AppState *)(*appstate);

    appstate_p->snake_game_manager = SnakeGameManager__init(GAME_SCENE_WIDTH, GAME_SCENE_HEIGHT);
    appstate_p->is_game_paused = false;
    appstate_p->is_game_speeding_up = false;
    appstate_p->tick_progression = 0.0f;

    appstate_p->window = SDL_CreateWindow("C Snake Game", APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT, SDL_WINDOW_INPUT_FOCUS);
    if (appstate_p->window == NULL)
    {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    appstate_p->renderer = SDL_CreateRenderer(appstate_p->window, NULL);
    if (appstate_p->renderer == NULL)
    {
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(appstate_p->window);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    render_gameplay_visuals(appstate_p->snake_game_manager,
                            appstate_p->window,
                            appstate_p->renderer,
                            APP_GAME_MAP_MARGIN_PX,
                            APP_GAME_MAP_MARGIN_PX,
                            appstate_p->is_game_paused);
    appstate_p->previous_tick = SDL_GetTicks();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *appstate_p = (AppState *)(appstate);

    const Uint64 now = SDL_GetTicks();
    const Uint64 dt_ms = now - appstate_p->previous_tick;
    update_tick_progression(appstate_p, dt_ms); // WIP: revamp tick progression

    bool is_game_updated_this_iteration = false;
    while (appstate_p->tick_progression >= 1.0f)
    {
        is_game_updated_this_iteration = true;
        appstate_p->tick_progression = appstate_p->tick_progression - 1.0f;
        if (appstate_p->is_game_paused == false)
            SnakeGameManager__update(appstate_p->snake_game_manager);
    }

    if (is_game_updated_this_iteration)
    {
        render_gameplay_visuals(appstate_p->snake_game_manager,
                                appstate_p->window,
                                appstate_p->renderer,
                                APP_GAME_MAP_MARGIN_PX,
                                APP_GAME_MAP_MARGIN_PX,
                                appstate_p->is_game_paused);
    }

// WIP: MOST LIKELY THIS IS CAUSING A PROBLEM!!!
    SDL_Delay((Uint32)GAME_TARGET_TICK_PERIOD_MS / 2U); // MUST BE DIVIDED BY >= 2U; saves some CPU

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *appstate_p = (AppState *)(appstate);
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
    {
        const SDL_KeyboardEvent event_key = event->key;
        const SDL_Scancode scancode = event_key.scancode;
        switch (scancode)
        {
        case SDL_SCANCODE_ESCAPE:
            if (SnakeGameManager__is_game_won(appstate_p->snake_game_manager) ||
                SnakeGameManager__is_game_lost(appstate_p->snake_game_manager))
                return SDL_APP_SUCCESS;
            appstate_p->is_game_paused = !appstate_p->is_game_paused;
            break;
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            SnakeGameManager__set_direction(appstate_p->snake_game_manager, 'w');
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            SnakeGameManager__set_direction(appstate_p->snake_game_manager, 'a');
            break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
            SnakeGameManager__set_direction(appstate_p->snake_game_manager, 's');
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            SnakeGameManager__set_direction(appstate_p->snake_game_manager, 'd');
            break;
        case SDL_SCANCODE_SPACE:
            appstate_p->is_game_speeding_up = true;
            break;
        case SDL_SCANCODE_R:
            if (SnakeGameManager__is_game_lost(appstate_p->snake_game_manager) ||
                SnakeGameManager__is_game_won(appstate_p->snake_game_manager))
            {
                SnakeGameManager__deinit(appstate_p->snake_game_manager);
                appstate_p->snake_game_manager = SnakeGameManager__init(GAME_SCENE_WIDTH, GAME_SCENE_HEIGHT);
            }
        default:
            break;
        }
        break;
    }
    case SDL_EVENT_KEY_UP:
    {
        const SDL_KeyboardEvent event_key = event->key;
        const SDL_Scancode scancode = event_key.scancode;
        if (scancode == SDL_SCANCODE_SPACE)
            appstate_p->is_game_speeding_up = false;
    }
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (appstate != NULL)
    {
        AppState *as = (AppState *)(appstate);
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SnakeGameManager__deinit(as->snake_game_manager);
        free(as);
    }
}
