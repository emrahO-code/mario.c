#include <raylib.h>
#include <player.h>

#ifndef PLATFORM_H
#define PLATFORM_H

typedef struct Platform{
    Vector2 position;
    Vector2 size;
    Color color;
} Platform;

void draw_platform(Platform p);

Rectangle PlatToRect(Platform p);
bool check_plat_collision(Player player,Platform platforms[],int count, int radius, float *correctedY, float *correctedX);

#endif //PLATFORM_H
