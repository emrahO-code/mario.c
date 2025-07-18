#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include <stdbool.h>
#include "level.h"

#define MAX_ENEMIES 50

typedef enum {
    ENEMY_GOOMBA,
    ENEMY_KOOPA,
    ENEMY_PIRANHA_PLANT
} EnemyType;

typedef enum {
    ENEMY_WALKING,
    ENEMY_DEAD,
    ENEMY_SQUISHED
} EnemyState;

typedef struct {
    Rectangle rectangle;
    Vector2 velocity;
    EnemyType type;
    EnemyState state;
    bool moving_right;
    bool active;           // Is this enemy slot in use
    float death_timer;     // Timer for death animation

    // Animation
    int current_frame;
    float frame_timer;
    float frame_duration;
    int max_frames;
} Enemy;

typedef struct {
    Enemy enemies[MAX_ENEMIES];
    int enemy_count;
    Texture2D enemy_sprites;
} EnemyManager;

// Function declarations
EnemyManager create_enemy_manager(void);
void load_enemy_sprites(EnemyManager* manager);
void spawn_goomba(EnemyManager* manager, float x, float y);
void update_enemies(EnemyManager* manager, Level level, float dt);
void update_enemy_physics(Enemy* enemy, float dt);
bool check_enemy_level_collision(Enemy* enemy, Level level, float dt);
bool check_enemy_tile_collision(Enemy* enemy, Rectangle tile_rect);
void draw_enemies(EnemyManager manager);
void unload_enemy_manager(EnemyManager manager);

// Player-Enemy interaction
bool check_player_enemy_collision(Rectangle player_rect, EnemyManager* manager, bool* enemy_defeated);
void defeat_enemy(Enemy* enemy);

#endif