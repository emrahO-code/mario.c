#include "enemy.h"
#include "level.h"
#include <stdio.h>
#include <math.h>

EnemyManager create_enemy_manager(void) {
    EnemyManager manager;
    manager.enemy_count = 0;
    manager.enemy_sprites.id = 0;

    // Initialize all enemies as inactive
    for (int i = 0; i < MAX_ENEMIES; i++) {
        manager.enemies[i].active = false;
    }

    return manager;
}

void load_enemy_sprites(EnemyManager* manager) {
    manager->enemy_sprites = LoadTexture("assets/mario_enemy.png");
    if (manager->enemy_sprites.id == 0) {
        printf("Warning: Could not load enemy_sprites.png, using colored rectangles\n");
    } else {
        printf("Enemy sprites loaded successfully: %dx%d\n",
               manager->enemy_sprites.width, manager->enemy_sprites.height);
    }
}

void spawn_goomba(EnemyManager* manager, float x, float y) {
    // Find an inactive enemy slot
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!manager->enemies[i].active) {
            Enemy* enemy = &manager->enemies[i];

            // Set up Goomba properties
            enemy->rectangle.x = x;
            enemy->rectangle.y = y;
            enemy->rectangle.width = 32;  // Standard Goomba size
            enemy->rectangle.height = 32;

            enemy->velocity.x = -50;  // Move left at 50 pixels/second
            enemy->velocity.y = 0;

            enemy->type = ENEMY_GOOMBA;
            enemy->state = ENEMY_WALKING;
            enemy->moving_right = false;
            enemy->active = true;
            enemy->death_timer = 0;

            // Animation setup
            enemy->current_frame = 0;
            enemy->frame_timer = 0;
            enemy->frame_duration = 0.5f; // 500ms per frame
            enemy->max_frames = 2; // 2-frame walking animation

            manager->enemy_count++;
            printf("Spawned Goomba at (%.0f, %.0f)\n", x, y);
            return;
        }
    }
    printf("Warning: Cannot spawn Goomba - no available enemy slots\n");
}

void update_enemies(EnemyManager* manager, Level level, float dt) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (manager->enemies[i].active) {
            Enemy* enemy = &manager->enemies[i];

            switch (enemy->state) {
                case ENEMY_WALKING:
                    // Check for wall/pit collision and turn around if needed
                    if (check_enemy_level_collision(enemy, level, dt)) {
                        enemy->velocity.x = -enemy->velocity.x;
                        enemy->moving_right = !enemy->moving_right;
                    }
                    if (check_enemy_enemy_collision(enemy, manager)) {
                    enemy->velocity.x = -enemy->velocity.x;
                    enemy->moving_right = !enemy->moving_right;
                    }

                    // Update animation
                    enemy->frame_timer += dt;
                    if (enemy->frame_timer >= enemy->frame_duration) {
                        enemy->frame_timer = 0;
                        enemy->current_frame = (enemy->current_frame + 1) % enemy->max_frames;
                    }

                    // Apply physics
                    update_enemy_physics(enemy,level, dt);
                    break;

                case ENEMY_SQUISHED:
                case ENEMY_DEAD:
                    // Death timer
                    enemy->death_timer += dt;
                    if (enemy->death_timer >= 1.0f) { // Remove after 1 second
                        enemy->active = false;
                        manager->enemy_count--;
                    }
                    break;
            }
        }
    }
}

bool check_enemy_level_collision(Enemy* enemy, Level level, float dt) {
    Rectangle future_rect = enemy->rectangle;
    future_rect.x += enemy->velocity.x * dt;

    // Check horizontal collision with level tiles
    float top_y = future_rect.y;
    float bottom_y = future_rect.y + future_rect.height;

    if (enemy->velocity.x > 0) { // Moving right
        float right_x = future_rect.x + future_rect.width;

        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, right_x, y);
            if (tile != TILE_EMPTY) {
                return true; // Hit a wall
            }
        }

        // Check for pit ahead (no ground to walk on)
        float ground_check_x = right_x + 10; // Look a bit ahead
        float ground_check_y = enemy->rectangle.y + enemy->rectangle.height + 10; // Below feet
        TileType ground_tile = get_tile_at_position(level, ground_check_x, ground_check_y);
        if (ground_tile == TILE_EMPTY) {
            return true; // Pit ahead, turn around
        }

    } else if (enemy->velocity.x < 0) { // Moving left
        float left_x = future_rect.x;

        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, left_x, y);
            if (tile != TILE_EMPTY) {
                return true; // Hit a wall
            }
        }

        // Check for pit ahead (no ground to walk on)
        float ground_check_x = left_x - 10; // Look a bit ahead
        float ground_check_y = enemy->rectangle.y + enemy->rectangle.height + 10; // Below feet
        TileType ground_tile = get_tile_at_position(level, ground_check_x, ground_check_y);
        if (ground_tile == TILE_EMPTY) {
            return true; // Pit ahead, turn around
        }
    }

    return false;
}

void update_enemy_physics(Enemy* enemy, Level level, float dt) {
    // Apply gravity
    enemy->velocity.y += 1000 * dt; // Same gravity as player

    // Move horizontally
    enemy->rectangle.x += enemy->velocity.x * dt;

    // Check vertical collision with level tiles
    Rectangle future_rect = enemy->rectangle;
    future_rect.y += enemy->velocity.y * dt;

    bool on_ground = false;

    if (enemy->velocity.y > 0) { // Falling
        float bottom_y = future_rect.y + future_rect.height;
        float left_x = future_rect.x;
        float right_x = future_rect.x + future_rect.width;

        // Check tiles under enemy's feet
        for (float x = left_x; x < right_x; x += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, x, bottom_y);
            if (tile != TILE_EMPTY) {
                // Hit a platform/ground
                int tile_y = (int)(bottom_y / TILE_SIZE);
                enemy->rectangle.y = tile_y * TILE_SIZE - enemy->rectangle.height;
                enemy->velocity.y = 0;
                on_ground = true;
                break;
            }
        }
    }

    // Apply vertical movement if no collision
    if (!on_ground) {
        enemy->rectangle.y += enemy->velocity.y * dt;
    }
}

bool check_enemy_tile_collision(Enemy* enemy, Rectangle tile_rect) {
    return CheckCollisionRecs(enemy->rectangle, tile_rect);
}

void draw_enemies(EnemyManager manager) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (manager.enemies[i].active) {
            Enemy enemy = manager.enemies[i];

            if (manager.enemy_sprites.id != 0) {
                Rectangle source_rect = {0, 0, 16, 16}; // Default

                switch (enemy.type) {
                    case ENEMY_GOOMBA:
                        switch (enemy.state) {
                            case ENEMY_WALKING:
                                // Goomba walking animation (adjust coordinates for your sprite sheet)
                                if (enemy.current_frame == 0) {
                                    source_rect = (Rectangle){0, 16, 16, 16}; // Frame 1
                                } else {
                                    source_rect = (Rectangle){18, 16, 16, 16}; // Frame 2
                                }
                                break;
                            case ENEMY_SQUISHED:
                                source_rect = (Rectangle){36, 24, 16, 8}; // Squished sprite
                                break;
                            case ENEMY_DEAD:
                                source_rect = (Rectangle){48, 0, 16, 16}; // Dead sprite
                                break;
                        }
                        break;

                    case ENEMY_KOOPA:
                        // Add Koopa sprites later
                        source_rect = (Rectangle){0, 16, 16, 24};
                        break;

                    default:
                        break;
                }

                Rectangle dest_rect = {
                    enemy.rectangle.x,
                    enemy.rectangle.y,
                    enemy.rectangle.width,
                    enemy.rectangle.height
                };

                DrawTexturePro(manager.enemy_sprites, source_rect, dest_rect,
                              (Vector2){0, 0}, 0, WHITE);
            } else {
                // Fallback colored rectangles
                Color enemy_color;
                switch (enemy.type) {
                    case ENEMY_GOOMBA:
                        if (enemy.state == ENEMY_SQUISHED) {
                            enemy_color = DARKBROWN;
                        } else {
                            enemy_color = BROWN;
                        }
                        break;
                    case ENEMY_KOOPA:
                        enemy_color = GREEN;
                        break;
                    default:
                        enemy_color = RED;
                        break;
                }

                DrawRectangleRec(enemy.rectangle, enemy_color);
            }
        }
    }
}

bool check_player_enemy_collision(Rectangle player_rect, EnemyManager* manager, bool* enemy_defeated) {
    *enemy_defeated = false;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (manager->enemies[i].active && manager->enemies[i].state == ENEMY_WALKING) {
            Enemy* enemy = &manager->enemies[i];

            if (CheckCollisionRecs(player_rect, enemy->rectangle)) {
                // Check if player is jumping on enemy (player's bottom vs enemy's top)
                float player_bottom = player_rect.y + player_rect.height;
                float enemy_top = enemy->rectangle.y;

                if (player_bottom - 10 < enemy_top) { // Player is above enemy
                    defeat_enemy(enemy);
                    *enemy_defeated = true;
                    printf("Enemy defeated by jumping!\n");
                    return false; // No damage to player
                } else {
                    // Side collision - player takes damage
                    printf("Player hit by enemy!\n");
                    return true;
                }
            }
        }
    }

    return false;
}

bool check_enemy_enemy_collision(Enemy* enemy, EnemyManager* manager) {
    // Check collision with other walking enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (manager->enemies[i].active &&
            manager->enemies[i].state == ENEMY_WALKING &&
            &manager->enemies[i] != enemy) { // Don't check against self

            Enemy* other = &manager->enemies[i];

            if (CheckCollisionRecs(enemy->rectangle, other->rectangle)) {
                return true; // Collision detected
            }
            }
    }
    return false;
}

void spawn_goomba_at_tile(EnemyManager* manager, int tile_x, int tile_y) {
    float pixel_x = tile_x * TILE_SIZE;
    float pixel_y = tile_y * TILE_SIZE;
    spawn_goomba(manager, pixel_x, pixel_y);
}

void spawn_configured_enemies(EnemyManager* enemy_manager) {
    // Enemy spawn configuration for World 1-1
    EnemySpawn spawns[] = {
        {34,12, ENEMY_GOOMBA},
        {55,12, ENEMY_GOOMBA},
        {66,12, ENEMY_GOOMBA},
        {68,12, ENEMY_GOOMBA},
        {97,3, ENEMY_GOOMBA},
        {99,3, ENEMY_GOOMBA},
        {113,12, ENEMY_GOOMBA},
        {130,12, ENEMY_GOOMBA},
        {132,12, ENEMY_GOOMBA},
        {139,12, ENEMY_GOOMBA},
        {141,12, ENEMY_GOOMBA},
        {144,12, ENEMY_GOOMBA},
        {146,12, ENEMY_GOOMBA},
        {190,12, ENEMY_GOOMBA},
        {192,12, ENEMY_GOOMBA},
    };

    int spawn_count = sizeof(spawns) / sizeof(EnemySpawn);

    // Clear existing enemies first
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_manager->enemies[i].active = false;
    }
    enemy_manager->enemy_count = 0;

    // Spawn all configured enemies
    for (int i = 0; i < spawn_count; i++) {
        switch (spawns[i].type) {
        case ENEMY_GOOMBA:
            spawn_goomba_at_tile(enemy_manager, spawns[i].tile_x,spawns[i].tile_y);
            break;
        case ENEMY_KOOPA:
            // Add Koopa spawning later when implemented
            printf("Koopa spawning not yet implemented\n");
            break;
        case ENEMY_PIRANHA_PLANT:
            // Add Piranha Plant spawning later when implemented
            printf("Piranha Plant spawning not yet implemented\n");
            break;
        }
    }
}

void defeat_enemy(Enemy* enemy) {
    enemy->state = ENEMY_SQUISHED;
    enemy->velocity.x = 0;
    enemy->velocity.y = 0;
    enemy->death_timer = 0;

    // Flatten the enemy sprite and keep it on the ground
    float old_bottom = enemy->rectangle.y + enemy->rectangle.height; // Remember where the bottom was
    enemy->rectangle.height = 16; // Make it half height
    enemy->rectangle.y = old_bottom - enemy->rectangle.height; // Adjust Y to keep bottom in same place
}

void unload_enemy_manager(EnemyManager manager) {
    if (manager.enemy_sprites.id != 0) {
        UnloadTexture(manager.enemy_sprites);
    }
}