#include "player.h"
#include <stdio.h>
#include <math.h>

Player create_player(void) {
    Rectangle rectangle;
    rectangle.x = 0;
    rectangle.y = 400;
    rectangle.width = 32;   // Scaled up from 16x16 original
    rectangle.height = 32;  // Small Mario height

    Vector2 velocity;
    velocity.x = 0;
    velocity.y = 0;

    Player player;
    player.rectangle = rectangle;
    player.velocity = velocity;
    player.state = MARIO_SMALL;
    player.facing_right = true;
    player.on_ground = false;
    player.invincible = false;
    player.invincible_timer = 0.0f;

    // Animation setup
    player.sprite_sheet.id = 0; // Will be loaded after InitWindow
    player.anim_state = ANIM_IDLE;
    player.current_frame = 0;
    player.frame_timer = 0.0f;
    player.frame_duration = 0.15f; // 150ms per frame
    player.max_frames = 1;

    return player;
}

void load_player_sprites(Player* player) {
    player->sprite_sheet = LoadTexture("assets/mario_player.png");
    if (player->sprite_sheet.id == 0) {
        printf("Warning: Could not load mario_sprites.png, using colored rectangles\n");
    } else {
        printf("Mario sprites loaded successfully: %dx%d\n",
               player->sprite_sheet.width, player->sprite_sheet.height);
    }
}

void update_mario_size(Player* player) {
    float old_bottom = player->rectangle.y + player->rectangle.height;

    switch (player->state) {
        case MARIO_SMALL:
            player->rectangle.width = 32;
            player->rectangle.height = 32;
            break;
        case MARIO_SUPER:
        case MARIO_FIRE:
            player->rectangle.width = 32;
            player->rectangle.height = 64; // Twice as tall
            break;
    }

    // Keep Mario's feet at the same position when changing size
    player->rectangle.y = old_bottom - player->rectangle.height;
}

void update_player_animation(Player* player, float dt) {
    // Update invincibility timer
    if (player->invincible) {
        player->invincible_timer -= dt;
        if (player->invincible_timer <= 0) {
            player->invincible = false;
            player->invincible_timer = 0;
        }
    }

    // Determine animation state based on movement
    if (!player->on_ground) {
        player->anim_state = ANIM_JUMPING;
        player->max_frames = 1;
    } else if (player->velocity.x != 0) {
        player->anim_state = ANIM_WALKING;
        player->max_frames = 3; // 3-frame walking animation
    } else {
        player->anim_state = ANIM_IDLE;
        player->max_frames = 1;
    }

    // Update animation timer
    player->frame_timer += dt;
    if (player->frame_timer >= player->frame_duration) {
        player->frame_timer = 0;
        player->current_frame = (player->current_frame + 1) % player->max_frames;
    }
}

void mario_take_damage(Player* player) {
    if (player->invincible) {
        return; // Can't take damage while invincible
    }

    switch (player->state) {
        case MARIO_SMALL:
            // Small Mario dies (you might want to handle this in main.c)
            printf("Mario died!\n");
            break;
        case MARIO_SUPER:
            // Super Mario becomes small
            player->state = MARIO_SMALL;
            update_mario_size(player);
            player->invincible = true;
            player->invincible_timer = 2.0f; // 2 seconds of invincibility
            printf("Mario powered down to small!\n");
            break;
        case MARIO_FIRE:
            // Fire Mario becomes super
            player->state = MARIO_SUPER;
            player->invincible = true;
            player->invincible_timer = 2.0f;
            printf("Mario powered down to super!\n");
            break;
    }
}

void mario_power_up(Player* player) {
    switch (player->state) {
        case MARIO_SMALL:
            // Small Mario becomes super
            player->state = MARIO_SUPER;
            update_mario_size(player);
            printf("Mario powered up to super!\n");
            break;
        case MARIO_SUPER:
            // Super Mario becomes fire (if fire flower)
            player->state = MARIO_FIRE;
            printf("Mario powered up to fire!\n");
            break;
        case MARIO_FIRE:
            // Already at max power - maybe give points or 1-up
            printf("Mario is already at max power!\n");
            break;
    }
}

void draw_player(Player player) {
    if (player.sprite_sheet.id != 0) {
        Rectangle source_rect = {0, 8, 16, 16}; // Default small Mario

        // Choose sprite based on Mario's state and animation
        switch (player.state) {
            case MARIO_SMALL:
                switch (player.anim_state) {
                    case ANIM_IDLE:
                        source_rect = (Rectangle){0, 8, 16, 16}; // Small Mario idle
                        break;
                    case ANIM_WALKING:
                        // Walking animation frames (adjust coordinates for your sprite sheet)
                        int walk_frames[] = {20, 38, 56}; // X coordinates for walk frames
                        source_rect = (Rectangle){walk_frames[player.current_frame], 8, 16, 16};
                        break;
                    case ANIM_JUMPING:
                        source_rect = (Rectangle){96, 8, 16, 16}; // Small Mario jumping
                        break;
                }
                break;

            case MARIO_SUPER:
                switch (player.anim_state) {
                    case ANIM_IDLE:
                        source_rect = (Rectangle){0, 32, 16, 32}; // Super Mario idle
                        break;
                    case ANIM_WALKING:
                        int walk_frames[] = {20, 38, 56};
                        source_rect = (Rectangle){walk_frames[player.current_frame], 32, 16, 32};
                        break;
                    case ANIM_JUMPING:
                        source_rect = (Rectangle){96, 32, 16, 32}; // Super Mario jumping
                        break;
                }
                break;

            case MARIO_FIRE:
                switch (player.anim_state) {
                    case ANIM_IDLE:
                        source_rect = (Rectangle){0, 140, 16, 32}; // Fire Mario idle
                        break;
                    case ANIM_WALKING:
                        int walk_frames[] = {20, 38, 56};
                        source_rect = (Rectangle){walk_frames[player.current_frame], 140, 16, 32};
                        break;
                    case ANIM_JUMPING:
                        source_rect = (Rectangle){96, 140, 16, 32}; // Fire Mario jumping
                        break;
                }
                break;
        }

        // Flip sprite horizontally if facing left
        if (!player.facing_right) {
            source_rect.width = -source_rect.width;
        }

        Rectangle dest_rect = {
            player.rectangle.x,
            player.rectangle.y,
            player.rectangle.width,
            player.rectangle.height
        };

        // Flash Mario during invincibility
        Color tint = WHITE;
        if (player.invincible) {
            // Flash effect - alternate between normal and transparent
            float flash_speed = 10.0f;
            float alpha = (sin(player.invincible_timer * flash_speed) + 1.0f) * 0.5f;
            tint = (Color){255, 255, 255, (unsigned char)(alpha * 255)};
        }

        DrawTexturePro(player.sprite_sheet, source_rect, dest_rect, (Vector2){0, 0}, 0, tint);
    } else {
        // Fallback colored rectangles if no sprite
        Color mario_color;
        switch (player.state) {
            case MARIO_SMALL: mario_color = GRAY; break;
            case MARIO_SUPER: mario_color = BLUE; break;
            case MARIO_FIRE: mario_color = RED; break;
            default: mario_color = GRAY; break;
        }

        // Flash during invincibility
        if (player.invincible) {
            float flash_speed = 10.0f;
            float alpha = (sin(player.invincible_timer * flash_speed) + 1.0f) * 0.5f;
            mario_color.a = (unsigned char)(alpha * 255);
        }

        DrawRectangleRec(player.rectangle, mario_color);
    }
}

void unload_player(Player player) {
    if (player.sprite_sheet.id != 0) {
        UnloadTexture(player.sprite_sheet);
    }
}