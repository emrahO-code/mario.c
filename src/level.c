#include "level.h"
#include "item.h"
#include "player.h"
#include <string.h>
#include <stdio.h>

Level create_world_1_1(void) {
    Level level;
    level.width = LEVEL_WIDTH;
    level.height = LEVEL_HEIGHT;
    level.coin_count = 0;

    // Don't load texture here - it will be loaded after InitWindow()
    level.tileset.id = 0; // Initialize as empty texture

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
    level.tiles[ground_row2 - 2][40] = TILE_PIPE_TOP_LEFT;
    level.tiles[ground_row2 - 2][41] = TILE_PIPE_TOP_RIGHT;

    level.tiles[ground_row2 - 1][40] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 1][41] = TILE_PIPE_BODY_RIGHT;
    // Question block at x=41
    level.tiles[ground_row2 - 3][53] = TILE_PIPE_TOP_LEFT;
    level.tiles[ground_row2 - 2][53] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 1][53] = TILE_PIPE_BODY_LEFT;

    level.tiles[ground_row2 - 3][54] = TILE_PIPE_TOP_RIGHT;
    level.tiles[ground_row2 - 2][54] = TILE_PIPE_BODY_RIGHT;
    level.tiles[ground_row2 - 1][54] = TILE_PIPE_BODY_RIGHT;

    level.tiles[ground_row2 - 4][61] = TILE_PIPE_TOP_LEFT;
    level.tiles[ground_row2 - 3][61] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 2][61] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 1][61] = TILE_PIPE_BODY_LEFT;

    level.tiles[ground_row2 - 4][62] = TILE_PIPE_TOP_RIGHT;
    level.tiles[ground_row2 - 3][62] = TILE_PIPE_BODY_RIGHT;
    level.tiles[ground_row2 - 2][62] = TILE_PIPE_BODY_RIGHT;
    level.tiles[ground_row2 - 1][62] = TILE_PIPE_BODY_RIGHT;

    level.tiles[ground_row2 - 4][73] = TILE_PIPE_TOP_LEFT;
    level.tiles[ground_row2 - 3][73] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 2][73] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 1][73] = TILE_PIPE_BODY_LEFT;

    level.tiles[ground_row2 - 4][74] = TILE_PIPE_TOP_RIGHT;
    level.tiles[ground_row2 - 3][74] = TILE_PIPE_BODY_RIGHT;
    level.tiles[ground_row2 - 2][74] = TILE_PIPE_BODY_RIGHT;
    level.tiles[ground_row2 - 1][74] = TILE_PIPE_BODY_RIGHT;

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

    for (int x = 107; x <= 109; x++) {
    level.tiles[high_block_level][x] = TILE_BRICK;  // Contains coin
    }

    // More bricks at x=115-117
    level.tiles[high_block_level][110] = TILE_QUESTION;
    level.tiles[block_level][110] = TILE_BRICK;

    level.tiles[block_level][116] = TILE_BRICK;
    level.tiles[block_level][117] = TILE_BRICK;

    level.tiles[block_level][122] = TILE_QUESTION;
    level.tiles[block_level][125] = TILE_QUESTION;
    level.tiles[block_level][128] = TILE_QUESTION;
    level.tiles[high_block_level][125] = TILE_QUESTION;

    level.tiles[block_level][134] = TILE_BRICK;

    for(int x = 137; x <= 139; x++) {
    level.tiles[high_block_level][x] = TILE_BRICK;
    }

    level.tiles[high_block_level][144] = TILE_BRICK;
    level.tiles[high_block_level][145] = TILE_QUESTION;
    level.tiles[high_block_level][146] = TILE_QUESTION;
    level.tiles[high_block_level][147] = TILE_BRICK;

    level.tiles[block_level][145] = TILE_BRICK;
    level.tiles[block_level][146] = TILE_BRICK;

    for(int x = 169; x <= 170; x++) {
        level.tiles[ground_row1][x] = TILE_EMPTY;
        level.tiles[ground_row2][x] = TILE_EMPTY;
    }

    for(int x = 0; x <= 3; x++) {
        for(int y = 0; y <= x; y++) {
            level.tiles[ground_row2 - 1 - y][150 + x] = TILE_BLOCK;
        }
    }

    for(int x = 0; x <= 3; x++) {
        // Fill from the ground up to the step height
        for(int y = x; y >= 0; y--) {
            level.tiles[ground_row2 - 1 - y][159 - x] = TILE_BLOCK;
        }
    }

    for(int x = 0; x <= 3; x++) {
        for(int y = 0; y <= x; y++) {
            level.tiles[ground_row2 - 1 - y][164 + x] = TILE_BLOCK;
        }
    }

    for(int x = 0; x <= 3; x++) {
        level.tiles[ground_row2-1-x][168] = TILE_BLOCK;
    }

    for(int x = 0; x <= 3; x++) {
        // Fill from the ground up to the step height
        for(int y = x; y >= 0; y--) {
            level.tiles[ground_row2 - 1 - y][174 - x] = TILE_BLOCK;
        }
    }

    level.tiles[ground_row2 - 2][179] = TILE_PIPE_TOP_LEFT;
    level.tiles[ground_row2 - 2][180] = TILE_PIPE_TOP_RIGHT;

    level.tiles[ground_row2 - 1][179] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 1][180] = TILE_PIPE_BODY_RIGHT;

    level.tiles[block_level][184] = TILE_BRICK;
    level.tiles[block_level][185] = TILE_BRICK;
    level.tiles[block_level][186] = TILE_QUESTION;
    level.tiles[block_level][187] = TILE_BRICK;

    level.tiles[ground_row2 - 2][195] = TILE_PIPE_TOP_LEFT;
    level.tiles[ground_row2 - 2][196] = TILE_PIPE_TOP_RIGHT;

    level.tiles[ground_row2 - 1][195] = TILE_PIPE_BODY_LEFT;
    level.tiles[ground_row2 - 1][196] = TILE_PIPE_BODY_RIGHT;

    for(int x = 0; x <= 7; x++) {
        for(int y = 0; y <= x; y++) {
            level.tiles[ground_row2 - 1 - y][197 + x] = TILE_BLOCK;
        }
    }

    for(int x = 0; x <= 7; x++) {
        level.tiles[ground_row2-1-x][205] = TILE_BLOCK;
    }

    return level;
}

void load_level_textures(Level* level) {
    // Load tileset texture after InitWindow() is called
    level->tileset = LoadTexture("assets/mario_tiles.png");

    // If texture fails to load, we'll fall back to colored rectangles
    if (level->tileset.id == 0) {
        printf("Warning: Could not load mario_tiles.png, using colored rectangles\n");
    } else {
        printf("Tileset loaded successfully: %dx%d\n", level->tileset.width, level->tileset.height);
    }
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
        if (level.tileset.id != 0) {
            printf("Tileset loaded: %dx%d\n", level.tileset.width, level.tileset.height);
        }
        debug_printed = true;
    }

    // Draw ALL tiles - try sprites first, fallback to colored rectangles
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (level.tiles[y][x] != TILE_EMPTY) {
                Rectangle tile_rect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                // Try to use sprites if texture is loaded
                bool sprite_drawn = false;
                if (level.tileset.id != 0) {
                    Rectangle source_rect = {0, 0, 16, 16}; // Start with safe coordinates

                    // Use very simple, safe sprite coordinates
                    switch (level.tiles[y][x]) {
                        case TILE_GROUND:
                            source_rect = (Rectangle){0, 16, 15, 16};    // Top-left tile
                            break;
                        case TILE_BRICK:
                            source_rect = (Rectangle){17, 16, 16, 16};   // Second tile in top row
                            break;
                        case TILE_QUESTION:
                            source_rect = (Rectangle){298, 78, 16, 16};   // Third tile in top row
                            break;
                        case TILE_QUESTION_USED:
                            source_rect = (Rectangle){349, 78, 16, 16};   // Used question block
                            break;
                        case TILE_PIPE_TOP_LEFT:
                            source_rect = (Rectangle){119, 196, 16, 16};   // Fourth tile in top row
                            break;
                        case TILE_PIPE_TOP_RIGHT:
                            source_rect = (Rectangle){136, 196, 15, 16};
                            break;
                        case TILE_PIPE_BODY_LEFT:
                            source_rect = (Rectangle){119, 213, 16, 16};   // Fifth tile in top row
                            break;
                        case TILE_PIPE_BODY_RIGHT:
                            source_rect = (Rectangle){136, 213, 16, 16};   // Fifth tile in top row
                            break;
                        case TILE_BLOCK:
                            source_rect = (Rectangle){0, 33, 16, 16};
                            break;
                        default:
                            source_rect = (Rectangle){0, 0, 16, 16};    // Default to first tile
                            break;
                    }

                    // Extra safety check
                    if (source_rect.x >= 0 && source_rect.y >= 0 &&
                        source_rect.x + source_rect.width <= level.tileset.width &&
                        source_rect.y + source_rect.height <= level.tileset.height) {

                        DrawTexturePro(level.tileset, source_rect, tile_rect, (Vector2){0, 0}, 0, WHITE);
                        sprite_drawn = true;
                    }
                }

                // Fallback to colored rectangles if sprite didn't work
                if (!sprite_drawn) {
                    Color tile_color;
                    switch (level.tiles[y][x]) {
                        case TILE_GROUND: tile_color = BROWN; break;
                        case TILE_BRICK: tile_color = RED; break;
                        case TILE_QUESTION: tile_color = YELLOW; break;
                        case TILE_QUESTION_USED: tile_color = ORANGE; break;
                        case TILE_PIPE_TOP_LEFT: tile_color = GREEN; break;
                        case TILE_PIPE_TOP_RIGHT: tile_color = BLUE; break;
                        case TILE_PIPE_BODY_RIGHT: tile_color = DARKGREEN; break;
                        case TILE_PIPE_BODY_LEFT: tile_color = RED; break;
                        case TILE_FLAGPOLE: tile_color = GRAY; break;
                        case TILE_FLAG: tile_color = RED; break;
                        case TILE_CASTLE: tile_color = LIGHTGRAY; break;
                        default: tile_color = WHITE; break;
                    }

                    DrawRectangleRec(tile_rect, tile_color);
                }
            }
        }
    }
}

void unload_level(Level level) {
    if (level.tileset.id != 0) {
        UnloadTexture(level.tileset);
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

bool interact_with_block(Level* level, int tile_x, int tile_y, void* item_manager_ptr, void* player_ptr) {
    if (tile_x < 0 || tile_x >= LEVEL_WIDTH || tile_y < 0 || tile_y >= LEVEL_HEIGHT) {
        return false;
    }

    // Cast void pointer to ItemManager pointer
    ItemManager* item_manager = (ItemManager*)item_manager_ptr;
    Player* player = (Player*)player_ptr;


    TileType block_type = level->tiles[tile_y][tile_x];
    float pixel_x = tile_x * TILE_SIZE;
    float pixel_y = tile_y * TILE_SIZE;

    switch (block_type) {
    case TILE_BRICK:
        if(player->state == MARIO_SUPER || player->state == MARIO_FIRE){
        level->tiles[tile_y][tile_x] = TILE_EMPTY;
            if (tile_x == 117) { // Another specific block for star
            spawn_star(item_manager, pixel_x, pixel_y - 32);
            }
        return true;
        } else {
            return false;
        }
        break;

    case TILE_QUESTION:
        level->tiles[tile_y][tile_x] = TILE_QUESTION_USED;

        // Spawn different items based on Mario's state or block location
        if (tile_x == 33 || (tile_x ==125 && tile_y == 4) ) { // First question block - spawn mushroom
            spawn_mushroom(item_manager, pixel_x, pixel_y - 32);
        } else {
            // Regular blocks just give coins
            level->coin_count++;
        }

        printf("Question block opened at (%d, %d)!\n", tile_x, tile_y);
        return false; // Block remains but changed
        break;

    case TILE_QUESTION_USED:
        printf("Already used question block at (%d, %d)\n", tile_x, tile_y);
        return false;
        break;

    default:
        return false;
    }
}

TileType get_tile_at_position(Level level, float x, float y) {
    int tile_x = (int)(x / TILE_SIZE);
    int tile_y = (int)(y / TILE_SIZE);

    if (tile_x < 0 || tile_x >= LEVEL_WIDTH || tile_y < 0 || tile_y >= LEVEL_HEIGHT) {
        return TILE_EMPTY;
    }

    return level.tiles[tile_y][tile_x];
}