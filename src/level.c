#include "level.h"
#include "item.h"
#include "player.h"
#include <string.h>
#include <stdio.h>

// Level element structure for easy level creation
typedef struct {
    int x, y;
    TileType tile;
} LevelElement;

// Pipe structure for easy pipe creation
typedef struct {
    int x, y;
    int height;
} Pipe;

// Block group structure
typedef struct {
    int start_x, end_x, y;
    TileType tile;
} BlockGroup;

// Staircase structure
typedef struct {
    int start_x, y;
    int steps;
    bool ascending; // true for up, false for down
} Staircase;

// Item spawn location
typedef struct {
    int x, y;
    ItemType item_type;
} ItemSpawn;

// Level configuration
typedef struct {
    LevelElement* elements;
    int element_count;
    Pipe* pipes;
    int pipe_count;
    BlockGroup* block_groups;
    int block_group_count;
    Staircase* staircases;
    int staircase_count;
    ItemSpawn* item_spawns;
    int item_spawn_count;
    int flagpole_x;
} LevelConfig;

// Helper functions
void create_ground(Level* level) {
    int ground_row1 = LEVEL_HEIGHT - 1; // = 14
    int ground_row2 = LEVEL_HEIGHT - 2; // = 13

    // Extend ground to cover full level width
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        level->tiles[ground_row1][x] = TILE_GROUND;
        level->tiles[ground_row2][x] = TILE_GROUND;
    }
}

void create_pit(Level* level, int start_x, int width) {
    int ground_row1 = LEVEL_HEIGHT - 1;
    int ground_row2 = LEVEL_HEIGHT - 2;

    for (int x = start_x; x < start_x + width && x < LEVEL_WIDTH; x++) {
        level->tiles[ground_row1][x] = TILE_EMPTY;
        level->tiles[ground_row2][x] = TILE_EMPTY;
    }
}

void create_pipe(Level* level, Pipe pipe) {
    int ground_level = LEVEL_HEIGHT - 2; // Y=13 is where ground surface is

    // Calculate where pipe top should be (above ground)
    int pipe_top_y = ground_level - pipe.height;

    // Pipe top
    if (pipe_top_y >= 0 && pipe_top_y < LEVEL_HEIGHT) {
        level->tiles[pipe_top_y][pipe.x] = TILE_PIPE_TOP_LEFT;
        if (pipe.x + 1 < LEVEL_WIDTH) {
            level->tiles[pipe_top_y][pipe.x + 1] = TILE_PIPE_TOP_RIGHT;
        }
    }

    // Pipe body - fill from below top to just above ground
    for (int y = pipe_top_y + 1; y < ground_level; y++) {
        if (y >= 0 && y < LEVEL_HEIGHT) {
            level->tiles[y][pipe.x] = TILE_PIPE_BODY_LEFT;
            if (pipe.x + 1 < LEVEL_WIDTH) {
                level->tiles[y][pipe.x + 1] = TILE_PIPE_BODY_RIGHT;
            }
        }
    }
}

void create_block_group(Level* level, BlockGroup group) {
    for (int x = group.start_x; x <= group.end_x && x < LEVEL_WIDTH; x++) {
        if (group.y >= 0 && group.y < LEVEL_HEIGHT) {
            level->tiles[group.y][x] = group.tile;
        }
    }
}

void create_staircase(Level* level, Staircase stair) {
    int ground_row2 = LEVEL_HEIGHT - 2;

    for (int step = 0; step < stair.steps; step++) {
        int x = stair.ascending ? stair.start_x + step : stair.start_x - step;
        int height = step + 1;

        for (int y = 0; y < height; y++) {
            int tile_y = ground_row2 - 1 - y;
            if (x >= 0 && x < LEVEL_WIDTH && tile_y >= 0 && tile_y < LEVEL_HEIGHT) {
                level->tiles[tile_y][x] = TILE_BLOCK;
            }
        }
    }
}

void create_flagpole(Level* level, int flagpole_x) {
    int ground_row2 = LEVEL_HEIGHT - 2;

    // Flagpole pole (10 tiles tall)
    for (int y = ground_row2 - 10; y <= ground_row2 - 1; y++) {
        if (y >= 0 && flagpole_x < LEVEL_WIDTH) {
            level->tiles[y][flagpole_x] = TILE_FLAGPOLE;
        }
    }

    // Flag at top
    if (ground_row2 - 10 >= 0 && flagpole_x + 1 < LEVEL_WIDTH) {
        level->tiles[ground_row2 - 10][flagpole_x + 1] = TILE_FLAG;
    }
}

void place_level_elements(Level* level, LevelElement* elements, int count) {
    for (int i = 0; i < count; i++) {
        LevelElement elem = elements[i];
        if (elem.x >= 0 && elem.x < LEVEL_WIDTH && elem.y >= 0 && elem.y < LEVEL_HEIGHT) {
            level->tiles[elem.y][elem.x] = elem.tile;
        }
    }
}

Level create_level_from_config(LevelConfig config) {
    Level level;
    level.width = LEVEL_WIDTH;
    level.height = LEVEL_HEIGHT;
    level.coin_count = 0;
    level.tileset.id = 0;

    // Initialize as empty
    memset(level.tiles, TILE_EMPTY, sizeof(level.tiles));

    // Create ground
    create_ground(&level);

    // Place individual elements
    if (config.elements) {
        place_level_elements(&level, config.elements, config.element_count);
    }

    // Create pits (handled in elements for now)

    // Create pipes
    for (int i = 0; i < config.pipe_count; i++) {
        create_pipe(&level, config.pipes[i]);
    }

    // Create block groups
    for (int i = 0; i < config.block_group_count; i++) {
        create_block_group(&level, config.block_groups[i]);
    }

    // Create staircases
    for (int i = 0; i < config.staircase_count; i++) {
        create_staircase(&level, config.staircases[i]);
    }

    // Create flagpole
    if (config.flagpole_x > 0) {
        create_flagpole(&level, config.flagpole_x);
    }

    return level;
}

Level create_world_1_1(void) {
    // Use hardcoded values since C doesn't allow variables in static initializers
    // ground_row2 = LEVEL_HEIGHT - 2 = 13
    // block_level = ground_row2 - 5 = 8
    // high_block_level = ground_row2 - 9 = 4

    // Define level elements
    static LevelElement elements[] = {
        // Question blocks
        {28, 8, TILE_QUESTION},
        {33, 8, TILE_QUESTION},
        {35, 8, TILE_QUESTION},
        {94, 8, TILE_QUESTION},
        {110, 4, TILE_QUESTION},
        {122, 8, TILE_QUESTION},
        {125, 8, TILE_QUESTION},
        {128, 8, TILE_QUESTION},
        {125, 4, TILE_QUESTION},
        {145, 4, TILE_QUESTION},
        {146, 4, TILE_QUESTION},
        {186, 8, TILE_QUESTION},

        // Individual bricks
        {32, 8, TILE_BRICK},
        {34, 8, TILE_BRICK},
        {36, 8, TILE_BRICK},
        {34, 4, TILE_QUESTION}, // Hidden block
        {93, 8, TILE_BRICK},
        {95, 8, TILE_BRICK},
        {110, 8, TILE_BRICK},
        {116, 8, TILE_BRICK},
        {117, 8, TILE_BRICK},
        {134, 8, TILE_BRICK},
        {144, 4, TILE_BRICK},
        {147, 4, TILE_BRICK},
        {145, 8, TILE_BRICK},
        {146, 8, TILE_BRICK},
        {184, 8, TILE_BRICK},
        {185, 8, TILE_BRICK},
        {187, 8, TILE_BRICK},
		{168,12, TILE_BLOCK},
		{168, 11, TILE_BLOCK},
		{168, 10, TILE_BLOCK},
		{168, 9, TILE_BLOCK},
    };

    // Define pipes with correct heights
    static Pipe pipes[] = {
        {40, 2,2},   // Small pipe
        {53, 3,3},   // Medium pipe
        {61, 4,4},   // Large pipe
        {73, 4,4},   // Large pipe
        {179, 2,2},  // Small pipe
        {195, 2,2}   // Small pipe
    };

    // Define block groups
    static BlockGroup block_groups[] = {
        {96, 102, 4, TILE_BRICK},  // high_block_level = 4
        {107, 109, 4, TILE_BRICK},
        {137, 139, 4, TILE_BRICK}
    };

    // Define staircases
    static Staircase staircases[] = {
        {150, 12, 4, true},   // ground_row2 - 1 = 12
        {159, 12, 4, false},
        {164, 12, 4, true},
        {174, 12, 4, false},
        {197, 12, 8, true},
    };

    LevelConfig config = {
        .elements = elements,
        .element_count = sizeof(elements) / sizeof(LevelElement),
        .pipes = pipes,
        .pipe_count = sizeof(pipes) / sizeof(Pipe),
        .block_groups = block_groups,
        .block_group_count = sizeof(block_groups) / sizeof(BlockGroup),
        .staircases = staircases,
        .staircase_count = sizeof(staircases) / sizeof(Staircase),
        .item_spawns = NULL,
        .item_spawn_count = 0,
        .flagpole_x = 214  // Near end of extended level
    };

    Level level = create_level_from_config(config);

    // Create pits manually for now
    create_pit(&level, 85, 2);    // Pit at x=85-86
    create_pit(&level, 102, 3);   // Pit at x=102-104
    create_pit(&level, 169, 2);   // Pit at x=169-170

    return level;
}

// Rest of the functions remain the same
void load_level_textures(Level* level) {
    level->tileset = LoadTexture("assets/mario_tiles.png");
    if (level->tileset.id == 0) {
        printf("Warning: Could not load mario_tiles.png, using colored rectangles\n");
    } else {
        printf("Tileset loaded successfully: %dx%d\n", level->tileset.width, level->tileset.height);
    }
}

void draw_level(Level level, Camera2D camera) {
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

    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (level.tiles[y][x] != TILE_EMPTY) {
                Rectangle tile_rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                bool sprite_drawn = false;

                if (level.tileset.id != 0) {
                    Rectangle source_rect = {0, 0, 16, 16};

                    switch (level.tiles[y][x]) {
                        case TILE_GROUND: source_rect = (Rectangle){0, 16, 15, 16}; break;
                        case TILE_BRICK: source_rect = (Rectangle){17, 16, 16, 16}; break;
                        case TILE_QUESTION: source_rect = (Rectangle){298, 78, 16, 16}; break;
                        case TILE_QUESTION_USED: source_rect = (Rectangle){349, 78, 16, 16}; break;
                        case TILE_PIPE_TOP_LEFT: source_rect = (Rectangle){119, 196, 16, 16}; break;
                        case TILE_PIPE_TOP_RIGHT: source_rect = (Rectangle){136, 196, 15, 16}; break;
                        case TILE_PIPE_BODY_LEFT: source_rect = (Rectangle){119, 213, 16, 16}; break;
                        case TILE_PIPE_BODY_RIGHT: source_rect = (Rectangle){136, 213, 16, 16}; break;
                        case TILE_BLOCK: source_rect = (Rectangle){0, 33, 16, 16}; break;
                        case TILE_FLAGPOLE: source_rect = (Rectangle){136, 247, 16, 16}; break;
                        case TILE_FLAG: source_rect = (Rectangle){136, 230, 16, 16}; break;
                        default: source_rect = (Rectangle){0, 0, 16, 16}; break;
                    }

                    if (source_rect.x >= 0 && source_rect.y >= 0 &&
                        source_rect.x + source_rect.width <= level.tileset.width &&
                        source_rect.y + source_rect.height <= level.tileset.height) {
                        DrawTexturePro(level.tileset, source_rect, tile_rect, (Vector2){0, 0}, 0, WHITE);
                        sprite_drawn = true;
                    }
                }

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
    Vector2 corners[4] = {
        {player_rect.x, player_rect.y},
        {player_rect.x + player_rect.width, player_rect.y},
        {player_rect.x, player_rect.y + player_rect.height},
        {player_rect.x + player_rect.width, player_rect.y + player_rect.height}
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

    ItemManager* item_manager = (ItemManager*)item_manager_ptr;
    Player* player = (Player*)player_ptr;
    TileType block_type = level->tiles[tile_y][tile_x];
    float pixel_x = tile_x * TILE_SIZE;
    float pixel_y = tile_y * TILE_SIZE;

    switch (block_type) {
    case TILE_BRICK:
        if(player->state == MARIO_SUPER || player->state == MARIO_FIRE){
            level->tiles[tile_y][tile_x] = TILE_EMPTY;
            if (tile_x == 117) {
                spawn_star(item_manager, pixel_x, pixel_y - 32);
            }
            return true;
        } else {
            return false;
        }
        break;

    case TILE_QUESTION:
        level->tiles[tile_y][tile_x] = TILE_QUESTION_USED;
        if (tile_x == 33 || (tile_x == 125 && tile_y == 4)) {
            spawn_mushroom(item_manager, pixel_x, pixel_y - 32);
        } else {
            level->coin_count++;
        }
        printf("Question block opened at (%d, %d)!\n", tile_x, tile_y);
        return false;
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

bool check_flagpole_collision(Level level, Rectangle player_rect) {
    int player_tile_x = (int)((player_rect.x + player_rect.width/2) / TILE_SIZE);
    int player_tile_y = (int)((player_rect.y + player_rect.height/2) / TILE_SIZE);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int check_x = player_tile_x + dx;
            int check_y = player_tile_y + dy;

            if (check_x >= 0 && check_x < LEVEL_WIDTH &&
                check_y >= 0 && check_y < LEVEL_HEIGHT) {

                if (level.tiles[check_y][check_x] == TILE_FLAGPOLE ||
                    level.tiles[check_y][check_x] == TILE_FLAG) {
                    return true;
                }
            }
        }
    }
    return false;
}