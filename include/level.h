#ifndef LEVEL_H
#define LEVEL_H

#include <raylib.h>

#define LEVEL_WIDTH 200
#define LEVEL_HEIGHT 15
#define TILE_SIZE 32

typedef enum {
    TILE_EMPTY,
    TILE_GROUND,
    TILE_BRICK,
    TILE_QUESTION,
    TILE_PIPE_TOP,
    TILE_PIPE_BODY,
    TILE_FLAGPOLE,
    TILE_FLAG,
    TILE_CASTLE
} TileType;

typedef struct {
    TileType tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
    int width;
    int height;
} Level;

Level create_world_1_1(void);
void draw_level(Level level, Camera2D camera);
bool check_tile_collision(Rectangle player_rect, Level level, int* tile_x, int* tile_y);
TileType get_tile_at_position(Level level, float x, float y);

#endif