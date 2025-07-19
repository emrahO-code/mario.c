#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_ITEMS 20

typedef enum {
    ITEM_MUSHROOM,
    ITEM_FIRE_FLOWER,
    ITEM_STAR,
    ITEM_1UP_MUSHROOM,
    ITEM_COIN
} ItemType;

typedef enum {
    ITEM_SPAWNING,
    ITEM_ACTIVE,
    ITEM_COLLECTED
} ItemState;

typedef struct {
    Rectangle rectangle;
    Vector2 velocity;
    ItemType type;
    ItemState state;
    bool active;
    bool moving_right;
    float spawn_timer;
    float collection_timer;
    int current_frame;
    float frame_timer;
    float frame_duration;
    int max_frames;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int active_count;
    Texture2D item_sprites;
} ItemManager;

ItemManager create_item_manager(void);
void load_item_sprites(ItemManager* manager);
void spawn_mushroom(ItemManager* manager, float x, float y);
void spawn_star(ItemManager* manager, float x, float y);
void spawn_fire_flower(ItemManager* manager, float x, float y);
void spawn_1up_mushroom(ItemManager* manager, float x, float y);
void update_items(ItemManager* manager, void* level, float dt);
void update_item_physics(Item* item, void* level, float dt);
bool check_player_item_collision(Rectangle player_rect, ItemManager* manager, ItemType* collected_type);
void draw_items(ItemManager manager);
void unload_item_manager(ItemManager manager);

#endif