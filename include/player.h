#include <raylib.h>

#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player{
    Rectangle rectangle;
    Vector2 velocity;
} Player;

Player create_player();

void draw_player(Player p);

#endif //PLAYER_H
