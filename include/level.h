#ifndef LEVEL_H
#define LEVEL_H

#include <raylib.h>

#define LEVEL_WIDTH 230
#define LEVEL_HEIGHT 15
#define TILE_SIZE 32

// Remove this line: typedef struct ItemManager ItemManager;

typedef enum {
    TILE_EMPTY,
    TILE_GROUND,
    TILE_BRICK,
    TILE_QUESTION,
    TILE_QUESTION_USED,
    TILE_PIPE_TOP_LEFT,
    TILE_PIPE_TOP_RIGHT,
    TILE_PIPE_BODY_LEFT,
    TILE_PIPE_BODY_RIGHT,
    TILE_BLOCK,
    TILE_FLAGPOLE,
    TILE_FLAG,
    TILE_CASTLE
} TileType;

typedef struct {
    TileType tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
    int width;
    int height;
    Texture2D tileset;
    int coin_count;
} Level;

Level create_world_1_1(void);
void load_level_textures(Level* level);
void draw_level(Level level, Camera2D camera);
bool check_tile_collision(Rectangle player_rect, Level level, int* tile_x, int* tile_y);
TileType get_tile_at_position(Level level, float x, float y);
// Change this line to use void* instead of ItemManager*
bool interact_with_block(Level* level, int tile_x, int tile_y, void* item_manager, void* player);
void unload_level(Level level);

#endif