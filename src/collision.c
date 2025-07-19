#include "collision.h"
#include <stdio.h>

bool check_pit_collision(Player* player) {
    // Check if Mario has fallen below the ground level (into a pit)
    float pit_threshold = (LEVEL_HEIGHT - 1) * TILE_SIZE + 50; // A bit below ground level

    if (player->rectangle.y > pit_threshold) {
        printf("Mario fell into a pit! Game restarting...\n");
        return true;
    }
    return false;
}

bool check_horizontal_collision(Player* player, Level level, float dt) {
    Rectangle future_rect = player->rectangle;
    future_rect.x += player->velocity.x * dt;

    // Check left world boundary (invisible wall)
    if (future_rect.x < 0) {
        player->rectangle.x = 0;
        return true;
    }

    // Check left and right collision
    float top_y = future_rect.y + 2; // Small margin from top/bottom
    float bottom_y = future_rect.y + future_rect.height - 2;

    if (player->velocity.x > 0) { // Moving right
        float right_x = future_rect.x + future_rect.width;

        for (float y = top_y; y < bottom_y; y += TILE_SIZE/4) { // More check points
            TileType tile = get_tile_at_position(level, right_x, y);
            if (tile != TILE_EMPTY) {
                int tile_x = (int)(right_x / TILE_SIZE);
                player->rectangle.x = tile_x * TILE_SIZE - player->rectangle.width - 1; // Small gap
                return true;
            }
        }
    } else if (player->velocity.x < 0) { // Moving left
        float left_x = future_rect.x;

        for (float y = top_y; y < bottom_y; y += TILE_SIZE/4) { // More check points
            TileType tile = get_tile_at_position(level, left_x, y);
            if (tile != TILE_EMPTY) {
                int tile_x = (int)(left_x / TILE_SIZE);
                player->rectangle.x = (tile_x + 1) * TILE_SIZE + 1; // Small gap
                return true;
            }
        }
    }

    return false;
}

bool check_level_collision(Player* player, Level* level, ItemManager* item_manager, float dt) {
    bool on_ground = false;

    // Check collision with level tiles
    Rectangle future_rect = player->rectangle;
    future_rect.y += player->velocity.y * dt;

    // Check vertical collision (falling/jumping)
    if (player->velocity.y > 0) { // Falling
        // Check bottom collision
        float bottom_y = future_rect.y + future_rect.height;
        float left_x = future_rect.x + 2; // Small margin from edges
        float right_x = future_rect.x + future_rect.width - 2;

        // Check tiles under player's feet
        for (float x = left_x; x < right_x; x += TILE_SIZE/4) { // More check points
            TileType tile = get_tile_at_position(*level, x, bottom_y);
            if (tile != TILE_EMPTY) {
                // Calculate exact collision point
                int tile_y = (int)(bottom_y / TILE_SIZE);
                player->rectangle.y = tile_y * TILE_SIZE - player->rectangle.height;
                player->velocity.y = 0;
                on_ground = true;
                break;
            }
        }
    } else if (player->velocity.y < 0) { // Jumping up
        // Check top collision
        float top_y = future_rect.y;
        float left_x = future_rect.x + 2; // Small margin from edges
        float right_x = future_rect.x + future_rect.width - 2;

        // Check tiles above player's head
        for (float x = left_x; x < right_x; x += TILE_SIZE/4) { // More check points
            TileType tile = get_tile_at_position(*level, x, top_y);
            if (tile != TILE_EMPTY) {
                // Hit ceiling - check for interactive blocks
                int block_x = (int)(x / TILE_SIZE);
                int block_y = (int)(top_y / TILE_SIZE);

                // Try to interact with the block
                if (tile == TILE_BRICK || tile == TILE_QUESTION || tile == TILE_QUESTION_USED) {
                    interact_with_block(level, block_x, block_y, item_manager, player);
                }

                // Stop upward movement
                int tile_y = (int)(top_y / TILE_SIZE);
                player->rectangle.y = (tile_y + 1) * TILE_SIZE;
                player->velocity.y = 0;
                break;
            }
        }
    }

    return on_ground;
}

bool check_tile_collision_at_position(Level level, float x, float y) {
    TileType tile = get_tile_at_position(level, x, y);
    return (tile != TILE_EMPTY);
}

bool check_rectangle_collision(Rectangle rect1, Rectangle rect2) {
    return CheckCollisionRecs(rect1, rect2);
}

bool check_player_ground_collision(Player* player, Level level) {
    // Check if player is standing on solid ground
    float bottom_y = player->rectangle.y + player->rectangle.height + 1; // Slightly below feet
    float left_x = player->rectangle.x + 2;
    float right_x = player->rectangle.x + player->rectangle.width - 2;
    
    for (float x = left_x; x < right_x; x += TILE_SIZE/4) {
        if (check_tile_collision_at_position(level, x, bottom_y)) {
            return true;
        }
    }
    
    return false;
}

CollisionInfo get_collision_info(Rectangle moving_rect, Rectangle static_rect, Vector2 velocity) {
    CollisionInfo info = {0};
    
    if (!CheckCollisionRecs(moving_rect, static_rect)) {
        info.collided = false;
        return info;
    }
    
    info.collided = true;
    
    // Calculate overlap amounts
    float overlap_x = 0, overlap_y = 0;
    
    if (velocity.x > 0) { // Moving right
        overlap_x = (moving_rect.x + moving_rect.width) - static_rect.x;
        info.side = COLLISION_RIGHT;
    } else if (velocity.x < 0) { // Moving left
        overlap_x = static_rect.x + static_rect.width - moving_rect.x;
        info.side = COLLISION_LEFT;
    }
    
    if (velocity.y > 0) { // Moving down
        overlap_y = (moving_rect.y + moving_rect.height) - static_rect.y;
        if (overlap_y < overlap_x) {
            info.side = COLLISION_BOTTOM;
        }
    } else if (velocity.y < 0) { // Moving up
        overlap_y = static_rect.y + static_rect.height - moving_rect.y;
        if (overlap_y < overlap_x) {
            info.side = COLLISION_TOP;
        }
    }
    
    info.overlap_x = overlap_x;
    info.overlap_y = overlap_y;
    
    return info;
}

bool check_point_in_rectangle(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width &&
            point.y >= rect.y && point.y <= rect.y + rect.height);
}

Vector2 get_rectangle_center(Rectangle rect) {
    return (Vector2){
        rect.x + rect.width / 2,
        rect.y + rect.height / 2
    };
}

float get_distance_between_rectangles(Rectangle rect1, Rectangle rect2) {
    Vector2 center1 = get_rectangle_center(rect1);
    Vector2 center2 = get_rectangle_center(rect2);
    
    float dx = center2.x - center1.x;
    float dy = center2.y - center1.y;
    
    return sqrtf(dx * dx + dy * dy);
}

bool check_player_enemy_proximity(Player player, Rectangle enemy_rect, float distance_threshold) {
    float distance = get_distance_between_rectangles(player.rectangle, enemy_rect);
    return (distance <= distance_threshold);
}

TileCollisionResult check_tiles_in_rectangle(Level level, Rectangle rect) {
    TileCollisionResult result = {0};
    
    int start_x = (int)(rect.x / TILE_SIZE);
    int end_x = (int)((rect.x + rect.width) / TILE_SIZE);
    int start_y = (int)(rect.y / TILE_SIZE);
    int end_y = (int)((rect.y + rect.height) / TILE_SIZE);
    
    // Clamp to level bounds
    start_x = (start_x < 0) ? 0 : start_x;
    end_x = (end_x >= LEVEL_WIDTH) ? LEVEL_WIDTH - 1 : end_x;
    start_y = (start_y < 0) ? 0 : start_y;
    end_y = (end_y >= LEVEL_HEIGHT) ? LEVEL_HEIGHT - 1 : end_y;
    
    for (int y = start_y; y <= end_y; y++) {
        for (int x = start_x; x <= end_x; x++) {
            TileType tile = level.tiles[y][x];
            if (tile != TILE_EMPTY) {
                result.collision_count++;
                result.has_collision = true;
                
                // Store first collision details
                if (result.collision_count == 1) {
                    result.first_tile_x = x;
                    result.first_tile_y = y;
                    result.first_tile_type = tile;
                }
                
                // Check for specific tile types
                if (tile == TILE_BRICK) result.has_brick = true;
                if (tile == TILE_QUESTION) result.has_question = true;
                if (tile == TILE_FLAGPOLE || tile == TILE_FLAG) result.has_flagpole = true;
            }
        }
    }
    
    return result;
}