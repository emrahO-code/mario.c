#include <raylib.h>
#include <platform.h>

void draw_platform(Platform p){
    DrawRectangleV(p.position,p.size,p.color);
}

