#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "level.h"
#include "item.h"
#include <raylib.h>
#include <math.h>

// Collision side enumeration
typedef enum {
    COLLISION_NONE,
    COLLISION_TOP,
    COLLISION_BOTTOM,
    COLLISION_LEFT,
    COLLISION_RIGHT
} CollisionSide;

// Detailed collision information
typedef struct {
    bool collided;
    CollisionSide side;
    float overlap_x;
    float overlap_y;
} CollisionInfo;

// Tile collision result
typedef struct {
    bool has_collision;
    int collision_count;
    int first_tile_x;
    int first_tile_y;
    TileType first_tile_type;
    bool has_brick;
    bool has_question;
    bool has_flagpole;
} TileCollisionResult;

// Basic collision functions
bool check_pit_collision(Player* player);
bool check_horizontal_collision(Player* player, Level level, float dt);
bool check_level_collision(Player* player, Level* level, ItemManager* item_manager, float dt);

// Utility collision functions
bool check_tile_collision_at_position(Level level, float x, float y);
bool check_rectangle_collision(Rectangle rect1, Rectangle rect2);
bool check_player_ground_collision(Player* player, Level level);
bool check_point_in_rectangle(Vector2 point, Rectangle rect);
bool check_player_enemy_proximity(Player player, Rectangle enemy_rect, float distance_threshold);

// Advanced collision functions
CollisionInfo get_collision_info(Rectangle moving_rect, Rectangle static_rect, Vector2 velocity);
TileCollisionResult check_tiles_in_rectangle(Level level, Rectangle rect);

// Utility functions
Vector2 get_rectangle_center(Rectangle rect);
float get_distance_between_rectangles(Rectangle rect1, Rectangle rect2);

#endif