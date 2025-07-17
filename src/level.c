#include "level.h"
#include <string.h>
#include <stdio.h>

Level create_world_1_1(void) {
    Level level;
    level.width = LEVEL_WIDTH;
    level.height = LEVEL_HEIGHT;

    // Initialize everything as empty
    memset(level.tiles, TILE_EMPTY, sizeof(level.tiles));

    // Calculate ground position to be at bottom of screen
    // Screen height = 600, so ground should be at y = 600 - (2 * TILE_SIZE) = 536
    // In tile coordinates: ground_y = 536 / TILE_SIZE = 16.75, so use tiles 17-18
    int ground_row1 = LEVEL_HEIGHT - 1;  // Bottom row
    int ground_row2 = LEVEL_HEIGHT - 2;  // Second from bottom

    // Ground level - bottom 2 rows
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        level.tiles[ground_row1][x] = TILE_GROUND;
        level.tiles[ground_row2][x] = TILE_GROUND;
    }

    // All other elements positioned relative to ground (9 tiles above ground = row 4)
    int block_level = ground_row2 - 5;  // 9 tiles above ground
    int high_block_level = ground_row2 - 9;  // 13 tiles above ground

    // Accurate World 1-1 layout (scaled to fit better in our level)

    // Question block at x=16 (contains Mushroom/Fire Flower)
    level.tiles[block_level][28] = TILE_QUESTION;

    // Group of blocks at x=20-22 (Q-Brick-Q with hidden coin block above)
    level.tiles[block_level][32] = TILE_BRICK;
    level.tiles[block_level][33] = TILE_QUESTION;  // Contains coin
    level.tiles[block_level][34] = TILE_BRICK;
    level.tiles[block_level][35] = TILE_QUESTION;  // Contains coin
    level.tiles[block_level][36] = TILE_BRICK;
    level.tiles[high_block_level][34] = TILE_QUESTION;  // Hidden coin block (invisible)

    // First pipe at x=35 (can enter)
    for(int x = 40; x < 42; x++) {
    level.tiles[ground_row2 - 2][x] = TILE_PIPE_TOP;
    level.tiles[ground_row2 - 1][x] = TILE_PIPE_BODY;
    // Question block at x=41
    }
    for(int x = 53; x < 55; x++) {
    level.tiles[ground_row2 - 3][x] = TILE_PIPE_TOP;
    level.tiles[ground_row2 - 2][x] = TILE_PIPE_BODY;
    level.tiles[ground_row2 - 1][x] = TILE_PIPE_BODY;
    }

    for(int x = 61; x < 63; x++) {
        level.tiles[ground_row2 - 4][x] = TILE_PIPE_TOP;
        level.tiles[ground_row2 - 3][x] = TILE_PIPE_BODY;
        level.tiles[ground_row2 - 2][x] = TILE_PIPE_BODY;
        level.tiles[ground_row2 - 1][x] = TILE_PIPE_BODY;
    }

    for(int x = 73; x < 75; x++) {
        level.tiles[ground_row2 - 4][x] = TILE_PIPE_TOP;
        level.tiles[ground_row2 - 3][x] = TILE_PIPE_BODY;
        level.tiles[ground_row2 - 2][x] = TILE_PIPE_BODY;
        level.tiles[ground_row2 - 1][x] = TILE_PIPE_BODY;
    }

    for (int x = 85; x <= 86; x++) {
        level.tiles[ground_row1][x] = TILE_EMPTY;
        level.tiles[ground_row2][x] = TILE_EMPTY;
    }

    // More bricks at x=85-87
    level.tiles[block_level][93] = TILE_BRICK;
    level.tiles[block_level][94] = TILE_QUESTION;
    level.tiles[block_level][95] = TILE_BRICK;

    // Question blocks at x=92-93
    for(int x = 96; x < 103; x++) {
    level.tiles[high_block_level][x] = TILE_BRICK;
    }
    for (int x = 102; x <= 104; x++) {
        level.tiles[ground_row1][x] = TILE_EMPTY;
        level.tiles[ground_row2][x] = TILE_EMPTY;
    }

    // Question block at x=110
    level.tiles[block_level][110] = TILE_QUESTION;  // Contains coin

    // More bricks at x=115-117
    level.tiles[block_level][115] = TILE_BRICK;
    level.tiles[block_level][116] = TILE_BRICK;
    level.tiles[block_level][117] = TILE_BRICK;

    // Final question block at x=122
    level.tiles[block_level][122] = TILE_QUESTION;  // Contains coin

    // Staircase (classic 4-step staircase) at x=130-136
    // Step 1
    level.tiles[ground_row2 - 1][130] = TILE_GROUND;
    // Step 2
    level.tiles[ground_row2 - 1][131] = TILE_GROUND;
    level.tiles[ground_row2 - 2][131] = TILE_GROUND;
    // Step 3
    level.tiles[ground_row2 - 1][132] = TILE_GROUND;
    level.tiles[ground_row2 - 2][132] = TILE_GROUND;
    level.tiles[ground_row2 - 3][132] = TILE_GROUND;
    // Step 4 (highest)
    level.tiles[ground_row2 - 1][133] = TILE_GROUND;
    level.tiles[ground_row2 - 2][133] = TILE_GROUND;
    level.tiles[ground_row2 - 3][133] = TILE_GROUND;
    level.tiles[ground_row2 - 4][133] = TILE_GROUND;

    // Staircase going down
    level.tiles[ground_row2 - 1][134] = TILE_GROUND;
    level.tiles[ground_row2 - 2][134] = TILE_GROUND;
    level.tiles[ground_row2 - 3][134] = TILE_GROUND;

    level.tiles[ground_row2 - 1][135] = TILE_GROUND;
    level.tiles[ground_row2 - 2][135] = TILE_GROUND;

    level.tiles[ground_row2 - 1][136] = TILE_GROUND;

    // Final large pit at x=140-147
    for (int x = 140; x <= 147; x++) {
        level.tiles[ground_row1][x] = TILE_EMPTY;
        level.tiles[ground_row2][x] = TILE_EMPTY;
    }

    // FLAGPOLE AREA at x=152
    // Flagpole (represented as pipe for now - we'll need to add TILE_FLAGPOLE later)
    for (int y = ground_row2 - 10; y <= ground_row2 - 1; y++) {
        level.tiles[y][152] = TILE_PIPE_BODY;  // Flagpole
    }
    level.tiles[ground_row2 - 10][152] = TILE_PIPE_TOP;  // Flag at top

    // CASTLE AREA at x=158-165
    // Castle base
    for (int x = 158; x <= 165; x++) {
        level.tiles[ground_row2 - 1][x] = TILE_GROUND;
        level.tiles[ground_row2 - 2][x] = TILE_GROUND;
        level.tiles[ground_row2 - 3][x] = TILE_GROUND;
        level.tiles[ground_row2 - 4][x] = TILE_GROUND;
    }

    // Castle towers
    level.tiles[ground_row2 - 5][159] = TILE_GROUND;   // Left tower
    level.tiles[ground_row2 - 6][159] = TILE_GROUND;
    level.tiles[ground_row2 - 5][161] = TILE_GROUND;   // Middle tower
    level.tiles[ground_row2 - 6][161] = TILE_GROUND;
    level.tiles[ground_row2 - 7][161] = TILE_GROUND;
    level.tiles[ground_row2 - 5][163] = TILE_GROUND;   // Right tower
    level.tiles[ground_row2 - 6][163] = TILE_GROUND;

    // Castle door
    level.tiles[ground_row2 - 3][161] = TILE_EMPTY;   // Door opening
    level.tiles[ground_row2 - 2][161] = TILE_EMPTY;

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
                    case TILE_FLAGPOLE: tile_color = GRAY; break;
                    case TILE_FLAG: tile_color = RED; break;
                    case TILE_CASTLE: tile_color = LIGHTGRAY; break;
                    default: tile_color = WHITE; break;
                }

                DrawRectangleRec(tile_rect, tile_color);

                // Debug: Draw tile borders
                DrawRectangleLinesEx(tile_rect, 1, BLACK);

                // Special drawing for flagpole
                if (level.tiles[y][x] == TILE_PIPE_TOP && x == 152) {
                    // Draw flag at top of pole
                    Rectangle flag_rect = {x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE, TILE_SIZE/2, TILE_SIZE/2};
                    DrawRectangleRec(flag_rect, RED);
                }
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