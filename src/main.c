#include <stdio.h>
#include <raylib.h>
#include <player.h>
#include <camera.h>
#include <level.h>
#include <enemy.h>

bool check_pit_collision(Player* player) {
    // Check if Mario has fallen below the ground level (into a pit)
    float pit_threshold = (LEVEL_HEIGHT - 1) * TILE_SIZE + 50; // A bit below ground level

    if (player->rectangle.y > pit_threshold) {
        printf("Mario fell into a pit! Game restarting...\n");
        return true;
    }
    return false;
}

void reset_game(Player* player, Level* level, EnemyManager* enemy_manager) {
    // Reset player position and state
    player->rectangle.x = 380; // Starting X position
    player->rectangle.y = (LEVEL_HEIGHT - 4) * TILE_SIZE; // Starting Y position
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->state = MARIO_SMALL; // Reset to small Mario
    player->facing_right = true;
    player->invincible = false;
    player->invincible_timer = 0;
    update_mario_size(player); // Update collision box size

    // Reset level (recreate it to restore broken blocks and reset coin count)
    unload_level(*level);
    *level = create_world_1_1();

    // Reset enemies - deactivate all enemies and respawn them
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_manager->enemies[i].active = false;
    }
    enemy_manager->enemy_count = 0;

    // Respawn Goombas at their starting positions
    spawn_goomba_at_tile(enemy_manager, 34, (LEVEL_HEIGHT - 4));
    spawn_goomba_at_tile(enemy_manager, 55, (LEVEL_HEIGHT - 4));
    spawn_goomba_at_tile(enemy_manager, 66, (LEVEL_HEIGHT - 4));
    spawn_goomba_at_tile(enemy_manager, 68, (LEVEL_HEIGHT - 4));
    spawn_goomba_at_tile(enemy_manager, 96, (LEVEL_HEIGHT - 11));
    spawn_goomba_at_tile(enemy_manager, 98, (LEVEL_HEIGHT - 11));

}

bool check_level_collision(Player* player, Level* level, float dt) {
    bool on_ground = false;

    // Check collision with level tiles
    Rectangle future_rect = player->rectangle;
    future_rect.y += player->velocity.y * dt;

    // Check vertical collision (falling/jumping)
    if (player->velocity.y > 0) { // Falling
        // Check bottom collision
        float bottom_y = future_rect.y + future_rect.height;
        float left_x = future_rect.x;
        float right_x = future_rect.x + future_rect.width;

        // Check tiles under player's feet
        for (float x = left_x; x < right_x; x += TILE_SIZE/2) {
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
        float left_x = future_rect.x;
        float right_x = future_rect.x + future_rect.width;

        // Check tiles above player's head
        for (float x = left_x; x < right_x; x += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(*level, x, top_y);
            if (tile != TILE_EMPTY) {
                // Hit ceiling - check for interactive blocks
                int block_x = (int)(x / TILE_SIZE);
                int block_y = (int)(top_y / TILE_SIZE);

                // Try to interact with the block
                if (tile == TILE_BRICK || tile == TILE_QUESTION || tile == TILE_QUESTION_USED) {
                    interact_with_block(level, block_x, block_y);
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

bool check_horizontal_collision(Player* player, Level level, float dt) {
    Rectangle future_rect = player->rectangle;
    future_rect.x += player->velocity.x * dt;

    // Check left world boundary (invisible wall)
    if (future_rect.x < 0) {
        player->rectangle.x = 0;
        return true;
    }

    // Check left and right collision
    float top_y = future_rect.y;
    float bottom_y = future_rect.y + future_rect.height;

    if (player->velocity.x > 0) { // Moving right
        float right_x = future_rect.x + future_rect.width;

        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, right_x, y);
            if (tile != TILE_EMPTY) {
                int tile_x = (int)(right_x / TILE_SIZE);
                player->rectangle.x = tile_x * TILE_SIZE - player->rectangle.width;
                return true;
            }
        }
    } else if (player->velocity.x < 0) { // Moving left
        float left_x = future_rect.x;

        for (float y = top_y; y < bottom_y; y += TILE_SIZE/2) {
            TileType tile = get_tile_at_position(level, left_x, y);
            if (tile != TILE_EMPTY) {
                int tile_x = (int)(left_x / TILE_SIZE);
                player->rectangle.x = (tile_x + 1) * TILE_SIZE;
                return true;
            }
        }
    }

    return false;
}

int main()
{
    //Constants
    const int JUMPVELOCITY = -700;
    const int WALKSPEED = 300;
    const int GRAVITY = 1300;

    //SCREENSIZE INIT AND CAMERA
    Vector2 SCREENSIZE;
    SCREENSIZE.x = 800;
    SCREENSIZE.y = 600;
    Camera2D camera = create_camera((Vector2) {SCREENSIZE.x/2, SCREENSIZE.y/2});

    // Create level, player, and enemies
    Level level = create_world_1_1();
    Player player = create_player();
    EnemyManager enemy_manager = create_enemy_manager();

    // Fix player starting position - place above ground
    player.rectangle.x = 380; // Start a bit away from left edge
    player.rectangle.y = (LEVEL_HEIGHT - 4) * TILE_SIZE; // Above ground level

    //Init Window
    InitWindow(SCREENSIZE.x, SCREENSIZE.y, "Mario - World 1-1");
    SetTargetFPS(60);

    // Load all textures after window initialization
    load_level_textures(&level);
    load_player_sprites(&player);
    load_enemy_sprites(&enemy_manager);

    // Spawn Goombas throughout the level
    spawn_goomba_at_tile(&enemy_manager, 45, LEVEL_HEIGHT - 4);  // Tile 18, above ground
    spawn_goomba_at_tile(&enemy_manager, 50, LEVEL_HEIGHT - 4);  // Tile 37, above ground
    spawn_goomba_at_tile(&enemy_manager, 56, LEVEL_HEIGHT - 4);  // Tile 56, above ground
    spawn_goomba_at_tile(&enemy_manager, 75, LEVEL_HEIGHT - 4);  // Tile 75, above ground
    spawn_goomba_at_tile(&enemy_manager, 94, LEVEL_HEIGHT - 4);

    //Main game loop
    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Apply gravity
        player.velocity.y += GRAVITY * dt;

        // Handle input and direction
        player.velocity.x = 0; // Reset horizontal velocity

        if (IsKeyDown(KEY_A)) {
            player.velocity.x = -WALKSPEED;
            player.facing_right = false; // Face left
        }
        if (IsKeyDown(KEY_D)) {
            player.velocity.x = WALKSPEED;
            player.facing_right = true;  // Face right
        }

        // Check for pit collision (Mario falling into pits)
        if (check_pit_collision(&player)) {
            reset_game(&player, &level, &enemy_manager);
            // Reload textures after level reset
            load_level_textures(&level);
            load_player_sprites(&player);
            load_enemy_sprites(&enemy_manager);
        }

        // Check horizontal collision and move horizontally
        if (!check_horizontal_collision(&player, level, dt)) {
            player.rectangle.x += player.velocity.x * dt;
        }

        // Check vertical collision and handle jumping
        bool on_ground = check_level_collision(&player, &level, dt);
        player.on_ground = on_ground; // Update player's ground state

        // Apply vertical movement if no collision
        if (!on_ground && player.velocity.y != 0) {
            player.rectangle.y += player.velocity.y * dt;
        }

        // Jump only when on ground
        if (on_ground && IsKeyDown(KEY_W)) {
            player.velocity.y = JUMPVELOCITY;
        }

        // Update player animation
        update_player_animation(&player, dt);

        // Update enemies (now includes collision detection)
        update_enemies(&enemy_manager, level, dt);

        // Check player-enemy collision
        bool enemy_defeated;
        if (check_player_enemy_collision(player.rectangle, &enemy_manager, &enemy_defeated)) {
            // Player was hit by enemy - take damage
            mario_take_damage(&player);

            // If Mario is small and gets hit, restart the game
            if (player.state == MARIO_SMALL) {
                printf("Small Mario died! Restarting game...\n");
                reset_game(&player, &level, &enemy_manager);
                load_level_textures(&level);
                load_player_sprites(&player);
                load_enemy_sprites(&enemy_manager);
                continue; // Skip rest of frame processing
            }
        }
        if (enemy_defeated) {
            // Player defeated an enemy - give a little bounce
            if (player.velocity.y > -200) { // Only bounce if not already jumping high
                player.velocity.y = -300; // Small bounce
            }
        }

        // Update camera to follow player
        camera.target = (Vector2){player.rectangle.x + player.rectangle.width/2, 180};

        // Drawing
        BeginDrawing();

        Color sky = (Color){148, 148, 255, 255};
        ClearBackground(sky); // Sky color like Mario

        BeginMode2D(camera);

        // Draw level
        draw_level(level, camera);

        // Draw enemies
        draw_enemies(enemy_manager);

        // Draw player
        draw_player(player);

        EndMode2D();

        // Draw UI elements here (outside camera)
        DrawText("World 1-1", 10, 10, 20, BLACK);
        DrawText("WASD to move and jump", 10, 40, 16, BLACK);
        DrawText(TextFormat("Coins: %d", level.coin_count), 10, 70, 16, BLACK);

        // Show Mario's current state
        const char* mario_state_text;
        switch (player.state) {
            case MARIO_SMALL: mario_state_text = "Small Mario"; break;
            case MARIO_SUPER: mario_state_text = "Super Mario"; break;
            case MARIO_FIRE: mario_state_text = "Fire Mario"; break;
            default: mario_state_text = "Unknown"; break;
        }
        DrawText(mario_state_text, 10, 100, 16, BLACK);

        // Show enemy count
        DrawText(TextFormat("Enemies: %d", enemy_manager.enemy_count), 10, 130, 16, BLACK);

        EndDrawing();
    }

    // Clean up
    unload_level(level);
    unload_player(player);
    unload_enemy_manager(enemy_manager);
    CloseWindow();
    return 0;
}