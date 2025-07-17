#include "level.h"
#include <string.h>
#include <stdio.h>

Level create_world_1_1(void) {
    Level level;
    level.width = LEVEL_WIDTH;
    level.height = LEVEL_HEIGHT;

    // Initialize everything as empty
    memset(level.tiles, TILE_EMPTY, sizeof(level.tiles));

    // Ground level - bottom 2 rows (rows 13-14)
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        level.tiles[LEVEL_HEIGHT-1][x] = TILE_GROUND;
        level.tiles[LEVEL_HEIGHT-2][x] = TILE_GROUND;
    }

    // World 1-1 specific layout (scaled down to fit better)

    // Test: Put some blocks close to spawn
    level.tiles[9][5] = TILE_QUESTION;  // Very close to start
    level.tiles[9][8] = TILE_BRICK;     // Also close
    level.tiles[5][10] = TILE_QUESTION; // Higher up, close

    // First question block at x=16
    level.tiles[9][16] = TILE_QUESTION;

    // Group of blocks at x=20-22
    level.tiles[9][20] = TILE_QUESTION;
    level.tiles[9][21] = TILE_BRICK;
    level.tiles[9][22] = TILE_QUESTION;
    level.tiles[5][20] = TILE_QUESTION; // Higher question block

    // First brick block group at x=30-35 (moved much closer)
    level.tiles[9][30] = TILE_BRICK;
    level.tiles[9][31] = TILE_BRICK;
    level.tiles[9][32] = TILE_BRICK;
    level.tiles[9][33] = TILE_BRICK;
    level.tiles[9][34] = TILE_BRICK;
    level.tiles[9][35] = TILE_BRICK;

    // Question block above bricks
    level.tiles[5][31] = TILE_QUESTION;
    level.tiles[5][34] = TILE_QUESTION;

    // Second group of bricks at x=40-42
    level.tiles[9][40] = TILE_BRICK;
    level.tiles[9][41] = TILE_BRICK;
    level.tiles[9][42] = TILE_BRICK;

    // Higher brick blocks
    level.tiles[5][40] = TILE_BRICK;
    level.tiles[5][41] = TILE_BRICK;
    level.tiles[5][42] = TILE_BRICK;

    // Even higher brick block
    level.tiles[1][41] = TILE_BRICK;

    // Question block at x=50
    level.tiles[9][50] = TILE_QUESTION;

    // More bricks at x=55-57
    level.tiles[9][55] = TILE_BRICK;
    level.tiles[9][56] = TILE_BRICK;
    level.tiles[9][57] = TILE_BRICK;

    // Question block at x=65
    level.tiles[9][65] = TILE_QUESTION;
    level.tiles[5][65] = TILE_QUESTION;

    // Stairs near the end (x=70-75)
    // First step
    level.tiles[12][70] = TILE_GROUND;

    // Second step
    level.tiles[12][71] = TILE_GROUND;
    level.tiles[11][71] = TILE_GROUND;

    // Third step
    level.tiles[12][72] = TILE_GROUND;
    level.tiles[11][72] = TILE_GROUND;
    level.tiles[10][72] = TILE_GROUND;

    // Fourth step (highest)
    level.tiles[12][73] = TILE_GROUND;
    level.tiles[11][73] = TILE_GROUND;
    level.tiles[10][73] = TILE_GROUND;
    level.tiles[9][73] = TILE_GROUND;

    // Stairs going down
    level.tiles[12][74] = TILE_GROUND;
    level.tiles[11][74] = TILE_GROUND;
    level.tiles[10][74] = TILE_GROUND;

    level.tiles[12][75] = TILE_GROUND;
    level.tiles[11][75] = TILE_GROUND;

    level.tiles[12][76] = TILE_GROUND;

    // Gap in ground from x=80-85 (pit)
    for (int x = 80; x <= 85; x++) {
        level.tiles[LEVEL_HEIGHT-1][x] = TILE_EMPTY;
        level.tiles[LEVEL_HEIGHT-2][x] = TILE_EMPTY;
    }

    // Final pipe at x=90
    level.tiles[11][90] = TILE_PIPE_TOP;
    level.tiles[12][90] = TILE_PIPE_BODY;

    return level;
}

void draw_level(Level level, Camera2D camera) {
    // Debug: Count how many non-empty tiles we have
    static bool debug_printed = false;
    if (!debug_printed) {
        int tile_count = 0;
        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            for (int x = 0; x < LEVEL_WIDTH; x++) {
                if (level.tiles[y][x] != TILE_EMPTY) {
                    tile_count++;
                    if (tile_count < 10) { // Print first 10 tiles
                        printf("Tile at (%d,%d): type %d\n", x, y, level.tiles[y][x]);
                    }
                }
            }
        }
        printf("Total tiles: %d\n", tile_count);
        debug_printed = true;
    }

    // Draw ALL tiles to debug (no culling)
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (level.tiles[y][x] != TILE_EMPTY) {
                Rectangle tile_rect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                Color tile_color;
                switch (level.tiles[y][x]) {
                    case TILE_GROUND: tile_color = BROWN; break;
                    case TILE_BRICK: tile_color = RED; break;
                    case TILE_QUESTION: tile_color = YELLOW; break;
                    case TILE_PIPE_TOP: tile_color = GREEN; break;
                    case TILE_PIPE_BODY: tile_color = DARKGREEN; break;
                    default: tile_color = WHITE; break;
                }

                DrawRectangleRec(tile_rect, tile_color);

                // Debug: Draw tile borders
                DrawRectangleLinesEx(tile_rect, 1, BLACK);
            }
        }
    }
}

bool check_tile_collision(Rectangle player_rect, Level level, int* tile_x, int* tile_y) {
    // Check all four corners of the player rectangle
    Vector2 corners[4] = {
        {player_rect.x, player_rect.y},                                    // Top-left
        {player_rect.x + player_rect.width, player_rect.y},               // Top-right
        {player_rect.x, player_rect.y + player_rect.height},              // Bottom-left
        {player_rect.x + player_rect.width, player_rect.y + player_rect.height} // Bottom-right
    };

    for (int i = 0; i < 4; i++) {
        int tx = (int)(corners[i].x / TILE_SIZE);
        int ty = (int)(corners[i].y / TILE_SIZE);

        if (tx >= 0 && tx < LEVEL_WIDTH && ty >= 0 && ty < LEVEL_HEIGHT) {
            if (level.tiles[ty][tx] != TILE_EMPTY) {
                if (tile_x) *tile_x = tx;
                if (tile_y) *tile_y = ty;
                return true;
            }
        }
    }

    return false;
}

TileType get_tile_at_position(Level level, float x, float y) {
    int tile_x = (int)(x / TILE_SIZE);
    int tile_y = (int)(y / TILE_SIZE);

    if (tile_x < 0 || tile_x >= LEVEL_WIDTH || tile_y < 0 || tile_y >= LEVEL_HEIGHT) {
        return TILE_EMPTY;
    }

    return level.tiles[tile_y][tile_x];
}