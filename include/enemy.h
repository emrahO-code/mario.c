#include <raylib.h>
#include <player.h>

#ifndef ENEMY_H
#define ENEMY_H

typedef struct Enemy{
    Rectangle Hitbox;
    Vector2 Velocity
} Enemy;

create_enemy(float x, float y);

move_enemy();

draw_enemy(Enemy e);


#endif //ENEMY_H
