#include <raylib.h>

#ifndef PLATFORM_H
#define PLATFORM_H

typedef struct Platform{
    Vector2 position;
    Vector2 size;
    Color color;
} Platform;

void draw_platform(struct Platform p);

#endif //PLATFORM_H
