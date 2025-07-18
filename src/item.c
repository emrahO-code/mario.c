#include "item.h"
#include "level.h"  // Include level.h in the .c file instead
#include <stdio.h>
#include <math.h>

ItemManager create_item_manager(void) {
    ItemManager manager;
    manager.active_count = 0;
    manager.item_sprites.id = 0;
    
    // Initialize all items as inactive
    for (int i = 0; i < MAX_ITEMS; i++) {
        manager.items[i].active = false;
    }
    
    return manager;
}

void load_item_sprites(ItemManager* manager) {
    manager->item_sprites = LoadTexture("assets/mario_items.png");
    if (manager->item_sprites.id == 0) {
        printf("Warning: Could not load items.png, using colored rectangles\n");
    } else {
        printf("Item sprites loaded successfully: %dx%d\n", 
               manager->item_sprites.width, manager->item_sprites.height);
    }
}

void spawn_mushroom(ItemManager* manager, float x, float y) {
    // Find an inactive item slot
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!manager->items[i].active) {
            Item* item = &manager->items[i];
            
            item->rectangle.x = x;
            item->rectangle.y = y;
            item->rectangle.width = 32;
            item->rectangle.height = 32;
            
            item->velocity.x = 60;  // Move right slowly
            item->velocity.y = -100; // Pop up from block
            
            item->type = ITEM_MUSHROOM;
            item->state = ITEM_SPAWNING;
            item->active = true;
            item->moving_right = true;
            item->spawn_timer = 0;
            item->collection_timer = 0;
            
            // Animation setup
            item->current_frame = 0;
            item->frame_timer = 0;
            item->frame_duration = 0.2f;
            item->max_frames = 1; // Mushroom doesn't animate
            
            manager->active_count++;
            printf("Spawned Mushroom at (%.0f, %.0f)\n", x, y);
            return;
        }
    }
    printf("Warning: Cannot spawn Mushroom - no available item slots\n");
}

void spawn_star(ItemManager* manager, float x, float y) {
    // Find an inactive item slot
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!manager->items[i].active) {
            Item* item = &manager->items[i];
            
            item->rectangle.x = x;
            item->rectangle.y = y;
            item->rectangle.width = 32;
            item->rectangle.height = 32;
            
            item->velocity.x = 120; // Move faster than mushroom
            item->velocity.y = -200; // Big bounce
            
            item->type = ITEM_STAR;
            item->state = ITEM_SPAWNING;
            item->active = true;
            item->moving_right = true;
            item->spawn_timer = 0;
            item->collection_timer = 0;
            
            // Animation setup
            item->current_frame = 0;
            item->frame_timer = 0;
            item->frame_duration = 0.1f; // Fast animation
            item->max_frames = 4; // 4-frame spinning animation
            
            manager->active_count++;
            printf("Spawned Star at (%.0f, %.0f)\n", x, y);
            return;
        }
    }
    printf("Warning: Cannot spawn Star - no available item slots\n");
}

void spawn_fire_flower(ItemManager* manager, float x, float y) {
    // Find an inactive item slot
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!manager->items[i].active) {
            Item* item = &manager->items[i];
            
            item->rectangle.x = x;
            item->rectangle.y = y;
            item->rectangle.width = 32;
            item->rectangle.height = 32;
            
            item->velocity.x = 0;   // Fire flower doesn't move
            item->velocity.y = -100; // Pop up from block then stop
            
            item->type = ITEM_FIRE_FLOWER;
            item->state = ITEM_SPAWNING;
            item->active = true;
            item->moving_right = true;
            item->spawn_timer = 0;
            item->collection_timer = 0;
            
            // Animation setup
            item->current_frame = 0;
            item->frame_timer = 0;
            item->frame_duration = 0.3f;
            item->max_frames = 2; // 2-frame animation
            
            manager->active_count++;
            printf("Spawned Fire Flower at (%.0f, %.0f)\n", x, y);
            return;
        }
    }
}

void spawn_1up_mushroom(ItemManager* manager, float x, float y) {
    // Find an inactive item slot
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!manager->items[i].active) {
            Item* item = &manager->items[i];
            
            item->rectangle.x = x;
            item->rectangle.y = y;
            item->rectangle.width = 32;
            item->rectangle.height = 32;
            
            item->velocity.x = 60;  // Same as regular mushroom
            item->velocity.y = -100;
            
            item->type = ITEM_1UP_MUSHROOM;
            item->state = ITEM_SPAWNING;
            item->active = true;
            item->moving_right = true;
            item->spawn_timer = 0;
            item->collection_timer = 0;
            
            // Animation setup
            item->current_frame = 0;
            item->frame_timer = 0;
            item->frame_duration = 0.2f;
            item->max_frames = 1;
            
            manager->active_count++;
            printf("Spawned 1UP Mushroom at (%.0f, %.0f)\n", x, y);
            return;
        }
    }
}

void update_items(ItemManager* manager, void* level_ptr, float dt) {
    Level level = *(Level*)level_ptr; // Cast and dereference

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (manager->items[i].active) {
            Item* item = &manager->items[i];

            switch (item->state) {
                case ITEM_SPAWNING:
                    item->spawn_timer += dt;

                    // After spawning animation, become active
                    if (item->spawn_timer >= 0.5f) {
                        item->state = ITEM_ACTIVE;
                        if (item->type == ITEM_FIRE_FLOWER) {
                            item->velocity.x = 0; // Fire flower stops moving
                            item->velocity.y = 0;
                        }
                    }

                    // Update animation
                    item->frame_timer += dt;
                    if (item->frame_timer >= item->frame_duration) {
                        item->frame_timer = 0;
                        item->current_frame = (item->current_frame + 1) % item->max_frames;
                    }

                    // Apply physics during spawn
                    update_item_physics(item, level_ptr, dt);
                    break;

                case ITEM_ACTIVE:
                    // Update animation
                    item->frame_timer += dt;
                    if (item->frame_timer >= item->frame_duration) {
                        item->frame_timer = 0;
                        item->current_frame = (item->current_frame + 1) % item->max_frames;
                    }

                    // Special star bouncing behavior
                    if (item->type == ITEM_STAR) {
                        // Star bounces continuously
                        if (item->velocity.y == 0) { // Just hit ground
                            item->velocity.y = -300; // Bounce again
                        }
                    }

                    // Apply physics
                    update_item_physics(item, level_ptr, dt);
                    break;

                case ITEM_COLLECTED:
                    item->collection_timer += dt;
                    if (item->collection_timer >= 0.5f) {
                        item->active = false;
                        manager->active_count--;
                    }
                    break;
            }
        }
    }
}

void update_item_physics(Item* item, void* level_ptr, float dt) {
    Level level = *(Level*)level_ptr; // Cast and dereference
    // Apply gravity (except for fire flower when active)
    if (!(item->type == ITEM_FIRE_FLOWER && item->state == ITEM_ACTIVE)) {
        item->velocity.y += 800 * dt; // Lighter gravity than enemies
    }
    
    // Check horizontal collision with level tiles
    Rectangle future_rect = item->rectangle;
    future_rect.x += item->velocity.x * dt;
    
    // Check for walls and turn around (like Goombas)
    float top_y = future_rect.y;
    float bottom_y = future_rect.y + future_rect.height;
    
    if (item->velocity.x > 0) { // Moving right
        float right_x = future_rect.x + future_rect.width;
        
        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, right_x, y);
            if (tile != TILE_EMPTY) {
                item->velocity.x = -item->velocity.x;
                item->moving_right = !item->moving_right;
                break;
            }
        }
    } else if (item->velocity.x < 0) { // Moving left
        float left_x = future_rect.x;
        
        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, left_x, y);
            if (tile != TILE_EMPTY) {
                item->velocity.x = -item->velocity.x;
                item->moving_right = !item->moving_right;
                break;
            }
        }
    }
    
    // Move horizontally
    item->rectangle.x += item->velocity.x * dt;
    
    // Check vertical collision with level tiles
    Rectangle future_rect_y = item->rectangle;
    future_rect_y.y += item->velocity.y * dt;
    
    bool on_ground = false;
    
    if (item->velocity.y > 0) { // Falling
        float bottom_y = future_rect_y.y + future_rect_y.height;
        float left_x = future_rect_y.x;
        float right_x = future_rect_y.x + future_rect_y.width;
        
        // Check tiles under item
        for (float x = left_x; x < right_x; x += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, x, bottom_y);
            if (tile != TILE_EMPTY) {
                // Hit ground/platform
                int tile_y = (int)(bottom_y / TILE_SIZE);
                item->rectangle.y = tile_y * TILE_SIZE - item->rectangle.height;
                item->velocity.y = 0;
                on_ground = true;
                break;
            }
        }
    }
    
    // Apply vertical movement if no collision
    if (!on_ground) {
        item->rectangle.y += item->velocity.y * dt;
    }
}

bool check_player_item_collision(Rectangle player_rect, ItemManager* manager, ItemType* collected_type) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (manager->items[i].active && manager->items[i].state == ITEM_ACTIVE) {
            Item* item = &manager->items[i];
            
            if (CheckCollisionRecs(player_rect, item->rectangle)) {
                *collected_type = item->type;
                item->state = ITEM_COLLECTED;
                item->collection_timer = 0;
                
                printf("Player collected %s!\n", 
                    item->type == ITEM_MUSHROOM ? "Mushroom" :
                    item->type == ITEM_STAR ? "Star" :
                    item->type == ITEM_FIRE_FLOWER ? "Fire Flower" :
                    item->type == ITEM_1UP_MUSHROOM ? "1UP Mushroom" : "Unknown Item");
                
                return true;
            }
        }
    }
    
    return false;
}

void draw_items(ItemManager manager) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (manager.items[i].active && manager.items[i].state != ITEM_COLLECTED) {
            Item item = manager.items[i];
            
            if (manager.item_sprites.id != 0) {
                Rectangle source_rect = {0, 0, 16, 16}; // Default
                
                switch (item.type) {
                    case ITEM_MUSHROOM:
                        source_rect = (Rectangle){0, 8, 16, 16}; // Red mushroom
                        break;
                    case ITEM_FIRE_FLOWER:
                        // Animated fire flower
                        source_rect = (Rectangle){68, 8, 16, 16};
                        break;
                    case ITEM_STAR:
                        // Animated spinning star
                        source_rect = (Rectangle){143, 8, 15, 16};
                        break;
                    case ITEM_1UP_MUSHROOM:
                        source_rect = (Rectangle){0, 26, 16, 16}; // Green mushroom
                        break;
                    case ITEM_COIN:
                        source_rect = (Rectangle){180, 36, 8, 16}; // Coin
                        break;
                }
                
                Rectangle dest_rect = {
                    item.rectangle.x,
                    item.rectangle.y,
                    item.rectangle.width,
                    item.rectangle.height
                };
                
                DrawTexturePro(manager.item_sprites, source_rect, dest_rect, 
                              (Vector2){0, 0}, 0, WHITE);
            } else {
                // Fallback colored rectangles
                Color item_color;
                switch (item.type) {
                    case ITEM_MUSHROOM: item_color = RED; break;
                    case ITEM_FIRE_FLOWER: item_color = ORANGE; break;
                    case ITEM_STAR: item_color = YELLOW; break;
                    case ITEM_1UP_MUSHROOM: item_color = GREEN; break;
                    case ITEM_COIN: item_color = GOLD; break;
                    default: item_color = WHITE; break;
                }
                
                DrawRectangleRec(item.rectangle, item_color);
            }
        }
    }
}

void unload_item_manager(ItemManager manager) {
    if (manager.item_sprites.id != 0) {
        UnloadTexture(manager.item_sprites);
    }
}