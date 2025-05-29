#include <raylib.h>
#include <platform.h>
#include <player.h>
#include <math.h>

void draw_platform(Platform p){
    DrawRectangleV(p.position,p.size,p.color);
}

Rectangle PlatToRect(Platform p){
    Rectangle plat;
    plat.x = p.position.x;
    plat.y = p.position.y;
    plat.width = p.size.x;
    plat.height = p.size.y;
    return plat;
}

bool check_plat_collision(Player player,Platform platforms[],int count,int radius, float *correctedY, float *correctedX){
    for(int i=0; i<count;i++){
        Rectangle plat = PlatToRect(platforms[i]);

        if(fabs(player.rectangle.x-plat.x) < radius){
            if(CheckCollisionRecs(player.rectangle, plat)){
                if(player.rectangle.y<plat.y){
                    *correctedY = plat.y - player.rectangle.height;
                }
                if(player.rectangle.y>plat.y-plat.height){
                    *correctedY = plat.y + plat.height;
                }
                return true;
            }
        }
    }
    return false;
}

