#include "enemy.h"
#include "level.h"
#include <stdio.h>
#include <math.h>

EnemyManager create_enemy_manager(void) {
    EnemyManager manager;
    manager.enemy_count = 0;
    manager.enemy_sprites.id = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        manager.enemies[i].active = false;
    }
    return manager;
}

void load_enemy_sprites(EnemyManager* manager) {
    manager->enemy_sprites = LoadTexture("assets/mario_enemy.png");
    if (manager->enemy_sprites.id == 0) {
    } else {
        manager->enemy_sprites.width, manager->enemy_sprites.height;
    }
}

void spawn_goomba(EnemyManager* manager, float x, float y) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!manager->enemies[i].active) {
            Enemy* enemy = &manager->enemies[i];

            enemy->rectangle.x = x;
            enemy->rectangle.y = y;
            enemy->rectangle.width = 32;
            enemy->rectangle.height = 32;

            enemy->velocity.x = -50;
            enemy->velocity.y = 0;

            enemy->type = ENEMY_GOOMBA;
            enemy->state = ENEMY_WALKING;
            enemy->moving_right = false;
            enemy->active = true;
            enemy->death_timer = 0;

            enemy->current_frame = 0;
            enemy->frame_timer = 0;
            enemy->frame_duration = 0.5f;
            enemy->max_frames = 2;

            manager->enemy_count++;
            return;
        }
    }
}

void update_enemies(EnemyManager* manager, Level level, float dt) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (manager->enemies[i].active) {
            Enemy* enemy = &manager->enemies[i];

            switch (enemy->state) {
                case ENEMY_WALKING:
                    if (check_enemy_level_collision(enemy, level, dt)) {
                        enemy->velocity.x = -enemy->velocity.x;
                        enemy->moving_right = !enemy->moving_right;
                    }
                    if (check_enemy_enemy_collision(enemy, manager)) {
                    enemy->velocity.x = -enemy->velocity.x;
                    enemy->moving_right = !enemy->moving_right;
                    }
                    enemy->frame_timer += dt;
                    if (enemy->frame_timer >= enemy->frame_duration) {
                        enemy->frame_timer = 0;
                        enemy->current_frame = (enemy->current_frame + 1) % enemy->max_frames;
                    }
                    update_enemy_physics(enemy,level, dt);
                    break;

                case ENEMY_SQUISHED:
                case ENEMY_DEAD:
                    enemy->death_timer += dt;
                    if (enemy->death_timer >= 1.0f) {
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
    float top_y = future_rect.y;
    float bottom_y = future_rect.y + future_rect.height;
    if (enemy->velocity.x > 0) {
        float right_x = future_rect.x + future_rect.width;
        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, right_x, y);
            if (tile != TILE_EMPTY) {
                return true;
            }
        }
        float ground_check_x = right_x + 10;
        float ground_check_y = enemy->rectangle.y + enemy->rectangle.height + 10; // Below feet
        TileType ground_tile = get_tile_at_position(level, ground_check_x, ground_check_y);
        if (ground_tile == TILE_EMPTY) {
            return true;
        }
    } else if (enemy->velocity.x < 0) {
        float left_x = future_rect.x;
        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, left_x, y);
            if (tile != TILE_EMPTY) {
                return true;
            }
        }
        float ground_check_x = left_x - 10;
        float ground_check_y = enemy->rectangle.y + enemy->rectangle.height + 10;
        TileType ground_tile = get_tile_at_position(level, ground_check_x, ground_check_y);
        if (ground_tile == TILE_EMPTY) {
            return true;
        }
    }

    return false;
}

void update_enemy_physics(Enemy* enemy, Level level, float dt) {
    enemy->velocity.y += 1000 * dt;
    enemy->rectangle.x += enemy->velocity.x * dt;
    Rectangle future_rect = enemy->rectangle;
    future_rect.y += enemy->velocity.y * dt;
    bool on_ground = false;
    if (enemy->velocity.y > 0) {
        float bottom_y = future_rect.y + future_rect.height;
        float left_x = future_rect.x;
        float right_x = future_rect.x + future_rect.width;
        for (float x = left_x; x < right_x; x += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, x, bottom_y);
            if (tile != TILE_EMPTY) {
                int tile_y = (int)(bottom_y / TILE_SIZE);
                enemy->rectangle.y = tile_y * TILE_SIZE - enemy->rectangle.height;
                enemy->velocity.y = 0;
                on_ground = true;
                break;
            }
        }
    }
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
                Rectangle source_rect = {0, 0, 16, 16};
                switch (enemy.type) {
                    case ENEMY_GOOMBA:
                        switch (enemy.state) {
                            case ENEMY_WALKING:
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
                float player_bottom = player_rect.y + player_rect.height;
                float enemy_top = enemy->rectangle.y;
                if (player_bottom - 10 < enemy_top) {
                    defeat_enemy(enemy);
                    *enemy_defeated = true;
                    return false;
                } else {
                    return true;
                }
            }
        }
    }

    return false;
}

bool check_enemy_enemy_collision(Enemy* enemy, EnemyManager* manager) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (manager->enemies[i].active &&
            manager->enemies[i].state == ENEMY_WALKING &&
            &manager->enemies[i] != enemy) {
            Enemy* other = &manager->enemies[i];
            if (CheckCollisionRecs(enemy->rectangle, other->rectangle)) {
                return true;
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
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_manager->enemies[i].active = false;
    }
    enemy_manager->enemy_count = 0;

    for (int i = 0; i < spawn_count; i++) {
        switch (spawns[i].type) {
        case ENEMY_GOOMBA:
            spawn_goomba_at_tile(enemy_manager, spawns[i].tile_x,spawns[i].tile_y);
            break;
        case ENEMY_KOOPA:
            break;
        case ENEMY_PIRANHA_PLANT:
            break;
        }
    }
}

void defeat_enemy(Enemy* enemy) {
    enemy->state = ENEMY_SQUISHED;
    enemy->velocity.x = 0;
    enemy->velocity.y = 0;
    enemy->death_timer = 0;
    float old_bottom = enemy->rectangle.y + enemy->rectangle.height;
    enemy->rectangle.height = 16;
    enemy->rectangle.y = old_bottom - enemy->rectangle.height;
}

void unload_enemy_manager(EnemyManager manager) {
    if (manager.enemy_sprites.id != 0) {
        UnloadTexture(manager.enemy_sprites);
    }
}