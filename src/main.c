#include <stdio.h>
#include <raylib.h>
#include <player.h>
#include <camera.h>
#include <level.h>

bool check_level_collision(Player* player, Level level, float dt) {
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
            TileType tile = get_tile_at_position(level, x, bottom_y);
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
            TileType tile = get_tile_at_position(level, x, top_y);
            if (tile != TILE_EMPTY) {
                // Hit ceiling
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

    // Create level and player
    Level level = create_world_1_1();
    Player player = create_player();

    // Fix player starting position - place above ground
    player.rectangle.x = 380; // Start a bit away from left edge
    player.rectangle.y = (LEVEL_HEIGHT - 4) * TILE_SIZE; // Above ground level

    //Init Window
    InitWindow(SCREENSIZE.x, SCREENSIZE.y, "Mario - World 1-1");
    SetTargetFPS(60);

    load_level_textures(&level);

    //Main game loop
    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Apply gravity
        player.velocity.y += GRAVITY * dt;

        // Handle input
        player.velocity.x = 0; // Reset horizontal velocity

        if (IsKeyDown(KEY_A)) {
            player.velocity.x = -WALKSPEED;
        }
        if (IsKeyDown(KEY_D)) {
            player.velocity.x = WALKSPEED;
        }

        // Check horizontal collision and move horizontally
        if (!check_horizontal_collision(&player, level, dt)) {
            player.rectangle.x += player.velocity.x * dt;
        }

        // Check vertical collision and handle jumping
        bool on_ground = check_level_collision(&player, level, dt);

        // Apply vertical movement if no collision
        if (!on_ground && player.velocity.y != 0) {
            player.rectangle.y += player.velocity.y * dt;
        }

        // Jump only when on ground
        if (on_ground && IsKeyDown(KEY_W)) {
            player.velocity.y = JUMPVELOCITY;
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

        // Draw player
        draw_player(player);

        EndMode2D();

        // Draw UI elements here (outside camera)
        DrawText("World 1-1", 10, 10, 20, BLACK);
        DrawText("WASD to move and jump", 10, 40, 16, BLACK);
        DrawText(TextFormat("Player X: %.0f, Y: %.0f", player.rectangle.x, player.rectangle.y), 10, 70, 16, BLACK);
        DrawText(TextFormat("Camera X: %.0f, Y: %.0f", camera.target.x, camera.target.y), 10, 100, 16, BLACK);
        DrawText(TextFormat("Tiles X: %d-%d", (int)((camera.target.x - 400) / TILE_SIZE), (int)((camera.target.x + 400) / TILE_SIZE)), 10, 130, 16, BLACK);

        EndDrawing();
    }
    unload_level(level);
    CloseWindow();
    return 0;
}