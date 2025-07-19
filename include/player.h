#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef enum {
    MARIO_SMALL,
    MARIO_SUPER,
    MARIO_FIRE
} MarioState;

typedef enum {
    ANIM_IDLE,
    ANIM_WALKING,
    ANIM_JUMPING
} AnimationState;

typedef struct {
    Rectangle rectangle;
    Vector2 velocity;
    MarioState state;
    bool facing_right;
    bool on_ground;
    bool invincible;
    float invincible_timer;
    Texture2D sprite_sheet;
    AnimationState anim_state;
    int current_frame;
    float frame_timer;
    float frame_duration;
    int max_frames;
} Player;

Player create_player(void);
void load_player_sprites(Player* player);
void update_player_animation(Player* player, float dt);
void update_mario_size(Player* player);
void mario_take_damage(Player* player);
void mario_power_up(Player* player);
void draw_player(Player player);
void unload_player(Player player);

#endif