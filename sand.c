
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define GRID_WIDTH WIDTH / CELL_SIZE
#define GRID_HEIGHT HEIGHT / CELL_SIZE

#define CELL_SIZE 4

#define SAND_COLOR 0xCFCF04
#define WATER_COLOR 0x0458CF

typedef enum {
  CELL_EMPTY,
  CELL_SAND,
  CELL_WATER,
} Cell;

void clear_grid(Cell grid[GRID_HEIGHT][GRID_WIDTH],
                Cell next_grid[GRID_HEIGHT][GRID_WIDTH]) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      grid[y][x] = CELL_EMPTY;
      next_grid[y][x] = CELL_EMPTY;
    }
  }
  printf("Grid Cleared\n");
}

void draw_grid(Cell grid[GRID_HEIGHT][GRID_WIDTH], SDL_Surface *psurface) {
  SDL_FillRect(psurface, NULL, 0x000000); // clear screen
  SDL_Rect r;
  r.w = CELL_SIZE;
  r.h = CELL_SIZE;
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      if (grid[y][x] == CELL_SAND) {
        r.x = x * CELL_SIZE;
        r.y = y * CELL_SIZE;
        SDL_FillRect(psurface, &r, SAND_COLOR);
      } else if (grid[y][x] == CELL_WATER) {
        r.x = x * CELL_SIZE;
        r.y = y * CELL_SIZE;
        SDL_FillRect(psurface, &r, WATER_COLOR);
      }
    }
  }
}
int is_solid(int cell) {
  return cell == CELL_SAND; // more solid logic in here later on
}
void update_block(Cell grid[GRID_HEIGHT][GRID_WIDTH],
                  Cell next_grid[GRID_HEIGHT][GRID_WIDTH]) {
  for (int y = 0; y < GRID_HEIGHT; y++)
    for (int x = 0; x < GRID_WIDTH; x++)
      next_grid[y][x] = grid[y][x]; // start by copying the state
  for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      if (grid[y][x] == CELL_SAND) {

        //// TODO handle sand and water interaction

        // down
        if (y < GRID_HEIGHT - 1 && grid[y + 1][x] == CELL_EMPTY) {
          next_grid[y + 1][x] = CELL_SAND;
          next_grid[y][x] = CELL_EMPTY;
        }
        // down with water
        else if (y < GRID_HEIGHT - 1 && grid[y + 1][x] == CELL_WATER) {
          next_grid[y + 1][x] = CELL_SAND;
          next_grid[y][x] = CELL_WATER;
        }
        // down right
        else if (y < GRID_HEIGHT - 1 && x < GRID_WIDTH - 1 &&
                 grid[y + 1][x] != CELL_EMPTY &&
                 grid[y + 1][x + 1] == CELL_EMPTY) {
          next_grid[y + 1][x + 1] = CELL_SAND;
          next_grid[y][x] = CELL_EMPTY;
        }
        // down left
        else if (y < GRID_HEIGHT - 1 && x > 0 && grid[y + 1][x] != CELL_EMPTY &&
                 grid[y + 1][x - 1] == CELL_EMPTY) {
          next_grid[y + 1][x - 1] = CELL_SAND;
          next_grid[y][x] = CELL_EMPTY;
        }
      }
      // water simulation
      else if (grid[y][x] == CELL_WATER) {

        // down
        if (y < GRID_HEIGHT - 1 && grid[y + 1][x] == CELL_EMPTY) {
          next_grid[y + 1][x] = CELL_WATER;
          next_grid[y][x] = CELL_EMPTY;
        }

        // down right
        else if (y < GRID_HEIGHT - 1 && x < GRID_WIDTH - 1 &&
                 grid[y + 1][x] != CELL_EMPTY &&
                 grid[y + 1][x + 1] == CELL_EMPTY) {
          next_grid[y + 1][x + 1] = CELL_WATER;
          next_grid[y][x] = CELL_EMPTY;
        }
        // down left
        else if (y < GRID_HEIGHT - 1 && x > 0 && grid[y + 1][x] != CELL_EMPTY &&
                 grid[y + 1][x - 1] == CELL_EMPTY) {
          next_grid[y + 1][x - 1] = CELL_WATER;
          next_grid[y][x] = CELL_EMPTY;
        }

        //// TODO simulate right and left water flow
        // random right or left
        else if (x > 0 && x < GRID_WIDTH - 1 && grid[y + 1][x] != CELL_EMPTY &&
                 grid[y][x + 1] == CELL_EMPTY && grid[y][x - 1] == CELL_EMPTY) {
          int direction = rand() % 2;
          // one liner char *result = (rand() % 2) ? "right" : "left";
          if (direction == 1) {
            next_grid[y][x + 1] = CELL_WATER;
            next_grid[y][x] = CELL_EMPTY;

          } else {
            next_grid[y][x - 1] = CELL_WATER;
            next_grid[y][x] = CELL_EMPTY;
          }
        }
        // right
        else if (x < GRID_WIDTH - 1 && is_solid(grid[y + 1][x]) &&
                 grid[y][x + 1] == CELL_EMPTY) {

          next_grid[y][x + 1] = CELL_WATER;
          next_grid[y][x] = CELL_EMPTY;
        }
        // left
        else if (x > 0 && x < GRID_WIDTH && is_solid(grid[y + 1][x]) &&
                 grid[y][x - 1] == CELL_EMPTY) {
          next_grid[y][x - 1] = CELL_WATER;
          next_grid[y][x] = CELL_EMPTY;
        }
      }
    }
  }
}

void place_block(Cell grid[GRID_HEIGHT][GRID_WIDTH], int mx, int my,
                 int BLOCK) {
  int gx = mx / CELL_SIZE;
  int gy = my / CELL_SIZE;

  if (gx >= 0 && gx < GRID_WIDTH && gy >= 0 && gy < GRID_HEIGHT) {
    grid[gy][gx] = BLOCK;
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software"); // useless
  SDL_Window *pwindow =
      SDL_CreateWindow("Sand Game", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  SDL_Event event;
  bool is_running = true;

  SDL_Rect rect = {50, 50, 50, 50};
  SDL_FillRect(psurface, &rect, 0xFF0000);

  Cell grid[GRID_HEIGHT][GRID_WIDTH];
  Cell next_grid[GRID_HEIGHT][GRID_WIDTH];
  clear_grid(grid, next_grid);

  int current_block = CELL_SAND;
  int *pcurrent_block = &current_block;
  while (is_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        SDL_KeyCode key_pressed;
      case SDL_QUIT:
        is_running = false;
      case SDL_KEYDOWN: //<-- using semi colon here or defining the variables
                        // before assigning them after the case , as it can
                        // only be done in c23, i am using c99 i guess instead
                        // i can declare all vars at the begginging  of the
                        // scope which is probably easier, but more confusing
        key_pressed = event.key.keysym.sym;
        if (key_pressed == SDLK_ESCAPE) {
          is_running = false;
          break;
        }
        if (key_pressed == SDLK_e) {
          place_block(grid, 10, 10, CELL_SAND);
          break;
        }
        if (key_pressed == SDLK_SPACE) {
          clear_grid(grid, next_grid);
        }
        if (key_pressed == SDLK_1) {
          *pcurrent_block = CELL_SAND;
        }
        if (key_pressed == SDLK_2) {
          *pcurrent_block = CELL_WATER;
        }
      }
    }
    // the game loop
    int BLOCK = *pcurrent_block;
    int mx, my;
    Uint32 buttons = SDL_GetMouseState(&mx, &my);
    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      place_block(grid, mx, my, BLOCK);
    }
    update_block(grid, next_grid);
    for (int y = 0; y < GRID_HEIGHT; y++)
      for (int x = 0; x < GRID_WIDTH; x++)
        grid[y][x] = next_grid[y][x];

    draw_grid(grid, psurface);
    SDL_UpdateWindowSurface(pwindow);
  }
  return 0;
}
