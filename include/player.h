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
    MarioState state;           // Current Mario form (small/super/fire)
    bool facing_right;          // Direction Mario is facing
    bool on_ground;             // Is Mario touching the ground
    bool invincible;            // Invincibility frames after taking damage
    float invincible_timer;     // Timer for invincibility
    
    // Animation
    Texture2D sprite_sheet;     // Mario sprite texture
    AnimationState anim_state;  // Current animation
    int current_frame;          // Current animation frame
    float frame_timer;          // Timer for animation frames
    float frame_duration;       // How long each frame lasts
    int max_frames;             // Total frames in current animation
} Player;

// Function declarations
Player create_player(void);
void load_player_sprites(Player* player);
void update_player_animation(Player* player, float dt);
void update_mario_size(Player* player);
void mario_take_damage(Player* player);
void mario_power_up(Player* player);
void draw_player(Player player);
void unload_player(Player player);

#endif