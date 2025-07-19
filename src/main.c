#include <stdio.h>
#include <raylib.h>
#include <player.h>
#include <camera.h>
#include <level.h>
#include <enemy.h>
#include <item.h>
#include <collision.h>

// Game constants
typedef struct {
    int JUMP_VELOCITY;
    int WALK_SPEED;
    int GRAVITY;
    Vector2 SCREEN_SIZE;
} GameConstants;

// Game state
typedef struct {
    Level level;
    Player player;
    EnemyManager enemy_manager;
    ItemManager item_manager;
    Camera2D camera;
} GameState;

GameConstants create_constants(void) {
    return (GameConstants){
        .JUMP_VELOCITY = -700,
        .WALK_SPEED = 300,
        .GRAVITY = 1300,
        .SCREEN_SIZE = {800, 600}
    };
}

GameState create_game_state(GameConstants constants) {
    GameState game = {0};

    game.level = create_world_1_1();
    game.player = create_player();
    game.enemy_manager = create_enemy_manager();
    game.item_manager = create_item_manager();
    game.camera = create_camera((Vector2){constants.SCREEN_SIZE.x/2, constants.SCREEN_SIZE.y/2});

    // Set initial player position
    game.player.rectangle.x = 380;
    game.player.rectangle.y = (LEVEL_HEIGHT - 4) * TILE_SIZE;

    return game;
}

void load_game_assets(GameState* game) {
    load_level_textures(&game->level);
    load_player_sprites(&game->player);
    load_enemy_sprites(&game->enemy_manager);
    load_item_sprites(&game->item_manager);
    spawn_configured_enemies(&game->enemy_manager);
}

void reset_game(GameState* game) {
    // Reset player
    game->player.rectangle.x = 380;
    game->player.rectangle.y = (LEVEL_HEIGHT - 4) * TILE_SIZE;
    game->player.velocity = (Vector2){0, 0};
    game->player.state = MARIO_SMALL;
    game->player.facing_right = true;
    game->player.invincible = false;
    game->player.invincible_timer = 0;
    update_mario_size(&game->player);

    // Reset level
    unload_level(game->level);
    game->level = create_world_1_1();

    // Reset enemies
    spawn_configured_enemies(&game->enemy_manager);

    // Reset items
    for (int i = 0; i < MAX_ITEMS; i++) {
        game->item_manager.items[i].active = false;
    }
    game->item_manager.active_count = 0;

    // Reload assets
    load_game_assets(game);
    printf("Game reset! Mario is back at the start.\n");
}

bool handle_collisions(GameState* game, float dt) {
    // Check pit collision first
    if (check_pit_collision(&game->player)) {
        reset_game(game);
        return true; // Game was reset
    }

    // Handle horizontal collision
    if (!check_horizontal_collision(&game->player, game->level, dt)) {
        game->player.rectangle.x += game->player.velocity.x * dt;
    }

    // Handle vertical collision and ground detection
    bool on_ground = check_level_collision(&game->player, &game->level, &game->item_manager, dt);
    game->player.on_ground = on_ground;

    // Apply vertical movement if not on ground
    if (!on_ground && game->player.velocity.y != 0) {
        game->player.rectangle.y += game->player.velocity.y * dt;
    }

    return false; // No reset needed
}

void handle_interactions(GameState* game) {
    // Item collision
    ItemType collected_item;
    if (check_player_item_collision(game->player.rectangle, &game->item_manager, &collected_item)) {
        switch (collected_item) {
            case ITEM_MUSHROOM:
            case ITEM_FIRE_FLOWER:
                mario_power_up(&game->player);
                break;
            case ITEM_STAR:
                game->player.invincible = true;
                game->player.invincible_timer = 10.0f;
                printf("Star power! Mario is invincible!\n");
                break;
            case ITEM_1UP_MUSHROOM:
                printf("Extra life!\n");
                break;
            case ITEM_COIN:
                game->level.coin_count++;
                break;
        }
    }

    // Flagpole collision
    if (check_flagpole_collision(game->level, game->player.rectangle)) {
        printf("Mario reached the flagpole! Level complete!\n");
        reset_game(game);
        return;
    }

    // Enemy collision
    bool enemy_defeated;
    if (check_player_enemy_collision(game->player.rectangle, &game->enemy_manager, &enemy_defeated)) {
        if (!game->player.invincible) {
            MarioState old_state = game->player.state;
            mario_take_damage(&game->player);

            if (old_state == MARIO_SMALL) {
                printf("Small Mario died! Restarting game...\n");
                reset_game(game);
                return;
            }
        } else {
            printf("Mario is invincible - no damage taken!\n");
        }
    }

    if (enemy_defeated && game->player.velocity.y > -200) {
        game->player.velocity.y = -300; // Bounce
    }
}

void handle_input(GameState* game, GameConstants constants) {
    // Reset horizontal velocity
    game->player.velocity.x = 0;

    // Handle movement input
    if (IsKeyDown(KEY_A)) {
        game->player.velocity.x = -constants.WALK_SPEED;
        game->player.facing_right = false;
    }
    if (IsKeyDown(KEY_D)) {
        game->player.velocity.x = constants.WALK_SPEED;
        game->player.facing_right = true;
    }

    // Handle jump input
    if (game->player.on_ground && IsKeyDown(KEY_W)) {
        game->player.velocity.y = constants.JUMP_VELOCITY;
    }
}

void update_game_systems(GameState* game, float dt) {
    // Update player animation
    update_player_animation(&game->player, dt);

    // Update enemies
    update_enemies(&game->enemy_manager, game->level, dt);

    // Update items
    update_items(&game->item_manager, &game->level, dt);

    // Update camera to follow player
    game->camera.target = (Vector2){
        game->player.rectangle.x + game->player.rectangle.width/2,
        180
    };
}

void draw_game_world(GameState game) {
    BeginMode2D(game.camera);

    // Draw level
    draw_level(game.level, game.camera);

    // Draw enemies
    draw_enemies(game.enemy_manager);

    // Draw items
    draw_items(game.item_manager);

    // Draw player
    draw_player(game.player);

    EndMode2D();
}

void draw_ui(GameState game) {
    DrawText("World 1-1", 10, 10, 20, BLACK);
    DrawText("WASD to move and jump", 10, 40, 16, BLACK);
    DrawText(TextFormat("Coins: %d", game.level.coin_count), 10, 70, 16, BLACK);

    const char* mario_state_text;
    switch (game.player.state) {
        case MARIO_SMALL: mario_state_text = "Small Mario"; break;
        case MARIO_SUPER: mario_state_text = "Super Mario"; break;
        case MARIO_FIRE: mario_state_text = "Fire Mario"; break;
        default: mario_state_text = "Unknown"; break;
    }
    DrawText(mario_state_text, 10, 100, 16, BLACK);
    DrawText(TextFormat("Enemies: %d", game.enemy_manager.enemy_count), 10, 130, 16, BLACK);
    DrawText(TextFormat("Items: %d", game.item_manager.active_count), 10, 160, 16, BLACK);

    if (game.player.invincible) {
        DrawText(TextFormat("STAR POWER! %.1fs", game.player.invincible_timer), 10, 190, 16, YELLOW);
    }
}

void cleanup_game(GameState game) {
    unload_level(game.level);
    unload_player(game.player);
    unload_enemy_manager(game.enemy_manager);
    unload_item_manager(game.item_manager);
}

int main() {
    GameConstants constants = create_constants();

    InitWindow(constants.SCREEN_SIZE.x, constants.SCREEN_SIZE.y, "Mario - World 1-1");
    SetTargetFPS(60);

    GameState game = create_game_state(constants);
    load_game_assets(&game);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Apply gravity
        game.player.velocity.y += constants.GRAVITY * dt;

        // Handle input
        handle_input(&game, constants);

        // Handle collisions (returns true if game was reset)
        if (handle_collisions(&game, dt)) {
            continue; // Skip rest of frame if game was reset
        }

        // Handle interactions
        handle_interactions(&game);

        // Update game systems
        update_game_systems(&game, dt);

        // Draw everything
        BeginDrawing();
        ClearBackground((Color){148, 148, 255, 255});

        draw_game_world(game);
        draw_ui(game);

        EndDrawing();
    }

    cleanup_game(game);
    CloseWindow();

    return 0;
}